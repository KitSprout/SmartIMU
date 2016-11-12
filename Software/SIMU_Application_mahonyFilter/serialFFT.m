clear;

dataLens = 19;
dataType = 'single';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
f = struct;
f.fig = subplot(1, 1, 1);
f.length = 128;
f.window = [-10, 170, 0, 1.2];
f = plotFFT(f, 0, 0, 'init');

while ishandle(fig)
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];     % record data
        gyr  = s.dataBuffer( 1 : 3, end);
        acc  = s.dataBuffer( 4 : 6, end);
        mag  = s.dataBuffer( 7 : 9, end);
        att  = s.dataBuffer(10 : 12, end);
        q    = s.dataBuffer(13 : 16, end);
        time = s.dataBuffer(17 : 19, end);
        time(3) = fix(time(3) * 100);

        freq = s.getFreq([17, 18, 19], f.length);
        f = plotFFT(f, s.dataBuffer(1, end - f.length + 1 : end), freq, 0);

        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz] Gyr[%9.3f, %9.3f, %9.3f] Acc[%6.3f, %6.3f, %6.3f] Mag[%8.3f, %8.3f, %8.3f] Att[%7.3f, %8.3f, %8.3f]\n', s.packet.sequenceNum, packetLens, time, fix(freq), gyr, acc, mag, att);
    end
end

s.close();
