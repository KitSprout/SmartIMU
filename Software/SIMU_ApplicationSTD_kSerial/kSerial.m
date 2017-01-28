classdef kSerial < handle

properties (SetAccess = public)
    dataBuffer = [];
end

properties (SetAccess = private)
    serial;
    comPort = '';
    baudRate = 115200;
    inputBufferSize = 4096;

    packet = struct;
end

methods
%{
    ---- constructor
    s = creatSerial()                           % port = 'auto', baudRate = 115200, no delete instrfindall
    s = creatSerial(baudRate)                   % port = 'auto', set baudRate, no delete instrfindall
    s = creatSerial(port, baudRate)             % set port, set baudRate, no delete instrfindall
    s = creatSerial(baudRate, 'clear')          % port = 'auto', delete instrfindall
    s = creatSerial(port, baudRate, 'clear')    % set port, set baudRate, delete instrfindall
    port = 'COMx', 'auto', 'select'
%}
    function s = kSerial( varargin )
        switch nargin
            case 0
                port = 'auto';
                s.baudRate = 115200;
            case 1
                port = 'auto';
                s.baudRate = varargin{1};
            case 2
                if ischar(varargin{1})
                    port = varargin{1};
                    s.baudRate = varargin{2};
                else
                    port = 'auto';
                    s.baudRate = varargin{1};
                    if strcmp(varargin{2}, 'clear')
                        delete(instrfindall);
                    end
                end
            case 3
                port = varargin{1};
                s.baudRate = varargin{2};
                if strcmp(varargin{3}, 'clear')
                    delete(instrfindall);
                end
            otherwise
                error('input error!!');
        end
        info = instrhwinfo('serial');
        comPortList = info.AvailableSerialPorts;

        if strcmp(port, 'auto')
            s.comPort = char(comPortList(1));
        elseif strcmp(port, 'select')
            fprintf('-- serial port : ');
            for i = 1 : size(char(comPortList), 1)
                fprintf(['\t[%d] ', char(comPortList(i))], i);
            end
            comPort = input('\n');
            if isempty(comPort) || (comPort > size(char(comPortList), 1))
                s.comPort = comPortList(1);
            else
                s.comPort = comPortList(comPort);
            end
        elseif strncmp(port, 'COM', 3)
            s.comPort = port;
        else
            error('com port error!!');
        end

        s.serial = serial(s.comPort, 'BaudRate', s.baudRate, 'DataBits', 8, 'StopBits', 1, 'Parity', 'none', 'FlowControl', 'none');
        s.serial.ReadAsyncMode = 'continuous';
        s.serial.InputBufferSize = s.inputBufferSize;

        fprintf(['com port : ', s.comPort, '\n']);
        s.initPacket();
    end

    function open( s )
        fopen(s.serial);
    end

    function close( s )
        fclose(s.serial);
    end

    function data = read( s, nByte, type )
        data = fread(s.serial, nByte, type);
    end

    function nByte = write( s, data, type )
        fwrite(s.serial, data, type);
        nByte = length(data);
    end

    function setInputBufferSize( s, bufferSize )
        s.inputBufferSize = bufferSize;
        s.serial.InputBufferSize = s.inputBufferSize;
    end

    function setBaudRate( s, baudRate )
        s.baudRate = baudRate;
        s.serial.baudRate = s.baudRate;
    end

    % init packet parameter
    function initPacket( s )
        s.packet.dataType       = 0;
        s.packet.dataLengths    = 0;
        s.packet.data           = 0;

        s.packet.packetSize     = 0;
        s.packet.packetCount    = 0;

        s.packet.recvBufSize    = s.inputBufferSize * 2;
        s.packet.recvBufLengths = 0;
        s.packet.recvBuffer     = zeros(s.packet.recvBufSize, 1);

        s.packet.sequenceNum    = 0;
        s.packet.availableData  = 0;
        s.packet.availableIndex = zeros(1, 256);        % fix(s.packet.recvBufSize / s.packet.packetSize) + 1
    end

    function varargout = packetRecv( s, dataLens, dataType )

        % default output
        s.packet.data = [];
        s.packet.availableData = 0;

        % update packet information
        s.packet.dataLengths = dataLens;
        s.packet.dataType    = dataType;
        s.packet.packetSize  = 8 + dataLens * s.sizeof(dataType);

        % start to read
        nBytes = get(s.serial, 'BytesAvailable');
        if nBytes > 0
            readData = fread(s.serial, nBytes, 'uint8');
            s.packet.recvBuffer(s.packet.recvBufLengths + 1 : s.packet.recvBufLengths + nBytes) = readData;
            s.packet.recvBufLengths = s.packet.recvBufLengths + nBytes;

            if s.packet.recvBufLengths >= s.packet.packetSize
                packetIndex = s.packet.recvBufLengths - s.packet.packetSize + 1;
                s.packet.availableIndex(:, :) = 0;

                % find available packet
                while packetIndex > 0
                    if isequal(s.packet.recvBuffer(packetIndex : packetIndex + 1), uint8('KS')')  % check 'KS'
                        if isequal(s.packet.recvBuffer(packetIndex + s.packet.packetSize - 2 : packetIndex + s.packet.packetSize - 1), [13; 10])   % check '\r\n'
                            s.packet.availableIndex = [packetIndex, s.packet.availableIndex(1 : end - 1)];
                            packetIndex = packetIndex - s.packet.packetSize + 1;
                            s.packet.packetCount = s.packet.packetCount + 1;
                        end
                    end
                    packetIndex = packetIndex - 1;
                end

                % check available packet index
                indexLens = size(find(s.packet.availableIndex ~= 0), 2);
                if indexLens ~= 0

                    % get newest packet information
                    i = s.packet.availableIndex(indexLens);
                    s.packet.gType = s.getDataType(fix(s.packet.recvBuffer(i + 3) / 16));
                    s.packet.gLens = s.packet.recvBuffer(i + 2) + mod(s.packet.recvBuffer(i + 3), 16) * 256 + 1;
                    s.packet.sequenceNum = typecast(uint8(s.packet.recvBuffer(i + s.packet.packetSize - 4 : i + s.packet.packetSize - 3)), 'uint16');

                    % get data from buffer
                    s.packet.data = zeros(s.packet.dataLengths, indexLens);
                    for k = 1 : indexLens
                        j = s.packet.availableIndex(k);
                        s.packet.data(:, k) = typecast(uint8(s.packet.recvBuffer(j + 4 : j + s.packet.packetSize - 5)), s.packet.dataType);    % s.packet.gType
                    end

                    % update recv buffer & lengths
                    s.packet.recvBuffer = [s.packet.recvBuffer(i + s.packet.packetSize : end); zeros(i + s.packet.packetSize - 1, 1)];
                    s.packet.recvBufLengths = s.packet.recvBufLengths - (i + s.packet.packetSize - 1);

                    s.packet.availableData = indexLens;
                end
            end
        end
        varargout = { s.packet.data, s.packet.availableData };
    end

    function freq = getFreq( s, index, length )
        sec_s = s.dataBuffer(index(1), end - length + 1) * 60 + s.dataBuffer(index(2), end - length + 1) + s.dataBuffer(index(3), end - length + 1);
        sec_e = s.dataBuffer(index(1), end) * 60              + s.dataBuffer(index(2), end)              + s.dataBuffer(index(3), end);
        freq = length / (sec_e - sec_s);
    end
end

methods (Access = private)

    function type = getDataType( s, typeNum )
        switch typeNum
            case 0,   type = 'int8';
            case 1,   type = 'uint8';
            case 2,   type = 'int16';
            case 3,   type = 'uint16';
            case 4,   type = 'int32';
            case 5,   type = 'uint32';
            case 6,   type = 'int64';
            case 7,   type = 'uint64';
            case 10,  type = 'single';
            case 11,  type = 'double';
        end
    end

    function byte = sizeof( s, type )
        switch type
            case 'int8',    byte = 1;
            case 'uint8',   byte = 1;
            case 'int16',   byte = 2;
            case 'uint16',  byte = 2;
            case 'int32',   byte = 4;
            case 'uint32',  byte = 4;
            case 'int64',   byte = 8;
            case 'uint64',  byte = 8;
            case 'single',  byte = 4;
            case 'double',  byte = 8;
        end
    end
end

end
