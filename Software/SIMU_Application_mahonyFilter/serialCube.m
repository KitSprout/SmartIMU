clear;

dataLens = 19;
dataType = 'single';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 140, 1200, 600], 'color', 'w');

figSub = subplot(1, 1, 1);
cube = kCube([0, 0, 0], [1.5, 1.5, 0.5], [1.5, 1.5, 1.5]);   % origin, scale, window
cube.initCube(figSub, [315, 30]);           % view

while ishandle(figSub)
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

        freq = s.getFreq([17, 18, 19], 256);
        cube.qText.String = num2str([ time(1), time(2), time(3), ...
                                      freq, ...
                                      gyr(1), gyr(2), gyr(3), ...
                                      acc(1), acc(2), acc(3), ...
                                      mag(1), mag(2), mag(3), ...
                                      att(1), att(2), att(3), ...
                                      q(1), q(2), q(3), q(4) ], ...
                                     'TIME [ %02i:%02i:%02i ]\n\nFreq = %6.2fHz\n\ngyr.x = %9.4f\ngyr.y = %9.4f\ngyr.z = %9.4f\n\nacc.x = %8.5f\nacc.y = %8.5f\nacc.z = %8.5f\n\nmag.x = %8.3f\nmag.y = %8.3f\nmag.z = %8.3f\n\natt.x = %9.4f\natt.y = %9.4f\natt.z = %9.4f\n\nq0 = %8.5f\nq1 = %8.5f\nq2 = %8.5f\nq3 = %8.5f');
        cube.plotCube([0, 0, 0], q);  % origin, quaternion
        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz] Gyr[%9.3f, %9.3f, %9.3f] Acc[%6.3f, %6.3f, %6.3f] Mag[%8.3f, %8.3f, %8.3f] Att[%7.3f, %8.3f, %8.3f]\n', s.packet.sequenceNum, packetLens, time, fix(freq), gyr, acc, mag, att);
    end
end

s.close();
