close all
clear all

% ***********************
% **** set parameters
%comPort = 'COM5';
baudRate = 115200;
y_axisMax =  16000;
y_axisMin = -16000;
window_width = 800;
recvDataLens = 24;      % recvDataLens = int16Lens * 2 + 4
% ***********************

if rem((recvDataLens - 4), 2) > 0
    error('ERROR : recvDataLens must = int16Lens * 2 + 4');
end

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
window_d = window_w * 0.9;  % display window
window = -window_d;  
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
                    runtimes = runtimes + 1;
                    for j = 2 : 2 : recvDataLens - 4
                        dataCov(j/2) = unsigned2signed(recvData(data_s(i) + j - 1) * 2^8 + recvData(data_s(i) + j));
                    end
                    data(:, runtimes) = dataCov';
                    state = 1;
                end
            end
        end

        if state == 1
% {
            window = window + count;
            displayData = runtimes - window_d + 1;
            if displayData > 0
                times = [displayData : runtimes];
                plot(times, data(2, displayData : end), 'r', times, data(3, displayData : end), 'g', times, data(4, displayData : end), 'b');
            else
                times = [1 : runtimes];
                plot(times, data(2, :), 'r', times, data(3, :), 'g', times, data(4, :), 'b');
            end
            axis([window, window + window_w, y_axisMin, y_axisMax]);
            drawnow
            state = 0;
% }
% {
            fprintf('runtimes = %d\n', runtimes);
            fprintf('Acc.X = %d, Acc.Y = %d, Acc.Z = %d\n', data(2, end), data(3, end), data(4, end));
            fprintf('Gyr.X = %d, Gyr.Y = %d, Gyr.Z = %d\n', data(5, end), data(6, end), data(7, end));
            fprintf('Mag.X = %d, Mag.Y = %d, Mag.Z = %d\n', data(8, end), data(9, end), data(10, end));
% }
        end
    end
end

fclose(s);
delete(s);

data = double(data);

%{
% check packet loss rate
check = data(1, 2 : end) - data(1, 1 : end-1);
pl = 0;
for i = 1 : runtimes - 1
    if check(i) >= 2
        pl = pl + 1;
    end
end
plr = pl / (runtimes - 1) * 100;
fprintf('\ntotal packet = %d, packet loss = %d, rate = %.4f %%\n', runtimes, pl, plr);
%}
