clear;

dataLens = 9;
dataType = 'int16';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 8);
s.open();

osc = kSerial_Oscilloscope();
osc.setWindow(30000, -30000, 2000);
osc.curveChannel = [4, 5, 6];
osc.curveColor   = ['r', 'g', 'b'];
osc.curveScale   = [1, 1, 1];
osc.initOscilloscope(1);

while ishandle(osc.fig)
    [packetData, packetLens] = s.packetRecv(dataLens, dataType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        osc.updateOscilloscope(s);
        fprintf('[%5i][%2i]   Gyr[%6i, %6i, %6i]   Acc[%6i, %6i, %6i]   Mag[%6i, %6i, %6i]\n', s.packet.sequenceNum, packetLens, s.dataBuffer(:, end));
    end
end

s.close();
