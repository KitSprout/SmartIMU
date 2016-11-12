clear;

dataLens = 19;
dataType = 'single';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 140, 1200, 600], 'color', 'w');

figSub = subplot(1, 1, 1);
osc = kSerial_Oscilloscope();
osc.setWindow(20, -20, 1000);
osc.curveChannel = [1, 2, 3];
osc.curveColor   = ['r', 'g', 'b'];
osc.curveScale   = [1, 1, 1];
osc.initOscilloscope(figSub);

while ishandle(osc.fig)
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        gyr  = s.dataBuffer( 1 : 3, end);
        acc  = s.dataBuffer( 4 : 6, end);
        mag  = s.dataBuffer( 7 : 9, end);
        att  = s.dataBuffer(10 : 12, end);
        q    = s.dataBuffer(13 : 16, end);
        time = s.dataBuffer(17 : 19, end);
        time(3) = fix(time(3) * 100);

        freq = s.getFreq([17, 18, 19], 256);
        osc.updateOscilloscope(s);
        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz] Gyr[%9.3f, %9.3f, %9.3f] Acc[%6.3f, %6.3f, %6.3f] Mag[%8.3f, %8.3f, %8.3f] Att[%7.3f, %8.3f, %8.3f]\n', s.packet.sequenceNum, packetLens, time, fix(freq), gyr, acc, mag, att);
    end
end

s.close();
