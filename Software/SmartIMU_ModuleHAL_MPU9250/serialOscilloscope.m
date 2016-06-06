close all
clear all

% ***********************
% **** set parameters
%comPort = 'COM5';
baudRate = 115200;
y_axisMax =  1000;
y_axisMin = -1000;
window_width = 800;
recvDataLens = 24;      % recvDataLens = int16Lens * 2 + 4
% ***********************

% Auto find available serial port
delete(instrfindall);
info  = instrhwinfo('serial');
comPort = info.AvailableSerialPorts

% Serial config
s = serial(comPort, 'BaudRate', baudRate, 'DataBits', 8, 'StopBits', 1, 'Parity', 'none', 'FlowControl', 'none');
s.ReadAsyncMode = 'continuous';
fclose(s);
fopen(s);

% Init data
window_w = window_width;
window = window_w * (-0.9);  % display window
runtimes = 0;
recvData = 0;

fig1 = figure(1);
set(fig1, 'Position', [200, 100, 1600, 700], 'color', 'w');  % 1920*1080
signal = plot(runtimes, recvData);
axis([window, window + window_w, y_axisMin, y_axisMax]);
xlabel('runtimes');
ylabel('data');
grid on;
hold on;

state = 0;
%for i = 1 : 1000
while ishandle(fig1)
    n_bytes = get(s, 'BytesAvailable');
    if n_bytes == 0
        n_bytes = recvDataLens * 2;
    end
    recvData = fread(s, n_bytes, 'uint8');
    data_s = find(recvData == 83);

    dataNum = size(data_s, 1);
    if dataNum >= 1
        i = 1;
        while (i) & (dataNum >= 1)
            if (n_bytes - data_s(dataNum)) < recvDataLens
                dataNum = dataNum - 1;
            else
                i = 0;
            end
        end
    end

    if dataNum >= 1
        count = 0;
        for i = 1 : dataNum
            if (recvData(data_s(i) + recvDataLens - 2) == 13) & (recvData(data_s(i) + recvDataLens - 1) == 10)
                check = sum(recvData(data_s(i) + 1 : data_s(i) + recvDataLens - 4));
                if rem(check, 256) == recvData(data_s(i) + recvDataLens - 3)
                    count = count + 1;
                    runtimes(runtimes(end) + 1) = runtimes(end) + 1;
                    for j = 2 : 2 : recvDataLens - 4
                        data(j/2, runtimes(end)) = unsigned2signed(recvData(data_s(i) + j - 1) * 2^8 + recvData(data_s(i) + j));
                    end
                    state = 1;
                end
            end
        end

        if state == 1
%
%            plot(runtimes, data(2, :), 'r', runtimes, data(3, :), 'g', runtimes, data(4, :), 'b');  % acc
%            plot(runtimes, data(5, :), 'r', runtimes, data(6, :), 'g', runtimes, data(7, :), 'b');  % gyr
            plot(runtimes, data(8, :), 'r', runtimes, data(9, :), 'g', runtimes, data(10, :), 'b'); % mag
            axis([window, window + window_w, y_axisMin, y_axisMax]);
            window = window + count;
            drawnow
            state = 0;
%
%            sprintf('Acc.X = %d mg, Acc.Y = %d mg, Acc.Z = %d mg\nGyr.X = %d dps, Gyr.Y = %d dps, Gyr.Z = %d dps\nruntimes = %d', data(1, end), data(2, end), data(3, end), data(4, end), data(5, end), data(6, end), runtimes(end))
        end
    end
end

data = double(data);

fclose(s);
delete(s);
