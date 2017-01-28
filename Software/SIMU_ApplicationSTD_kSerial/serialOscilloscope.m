clear;

dataLens = 9;
dataType = 'int16';

s = kSerial(115200, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 140, 1200, 600], 'color', 'w');

figSub = subplot(1, 1, 1);
osc = kSerial_Oscilloscope();
osc.setWindow(8192*2, -8192*2, 1000);
osc.curveChannel = [4, 5, 6];
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
        osc.updateOscilloscope(s);
        fprintf('[%05i][%02i] Gyr[%6.0f, %6.0f, %6.0f] Acc[%6.0f, %6.0f, %6.0f] Mag[%6.0f, %6.0f, %6.0f]\n', s.packet.sequenceNum, packetLens, gyr, acc, mag);
    end
end

s.close();
