clear;

dataLens = 19;
dataType = 'single';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(dataLens, 1024 * 16);
s.open();

getFirstSequenceNum = true;
firstSequenceNum = 0;

tic
for i = 1 : 4000
%while true
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
        fprintf('[%05i][%02i][%02i:%02i:%02i][%3dHz] Gyr[%9.3f, %9.3f, %9.3f] Acc[%6.3f, %6.3f, %6.3f] Mag[%8.3f, %8.3f, %8.3f] Att[%7.3f, %8.3f, %8.3f]\n', s.packet.sequenceNum, packetLens, time, fix(freq), gyr, acc, mag, att);
        if getFirstSequenceNum
            firstSequenceNum    = s.packet.sequenceNum;
            getFirstSequenceNum = false;
        end
    end
end
time = toc;

lostPacket = (s.packet.sequenceNum - firstSequenceNum + 1) - s.packet.packetCount;
sampleRate = s.packet.packetCount / time;
fprintf('\nrecv packet = %d, lost packet = %d, sample rate = %.3f Hz\n', s.packet.packetCount, lostPacket, sampleRate);

s.close();
