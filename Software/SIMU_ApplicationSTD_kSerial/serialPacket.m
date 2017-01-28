clear;

dataLens = 9;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

getFirstSequenceNum = true;
firstSequenceNum = 0;

for i = 1 : 6000
%while true
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        gyr  = s.dataBuffer( 1 : 3, end);
        acc  = s.dataBuffer( 4 : 6, end);
        mag  = s.dataBuffer( 7 : 9, end);
        fprintf('[%05i][%02i] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f]\n', s.packet.sequenceNum, packetLens, gyr, acc, mag);
        if getFirstSequenceNum
            firstSequenceNum    = s.packet.sequenceNum;
            getFirstSequenceNum = false;
        end
    end
end

s.close();
