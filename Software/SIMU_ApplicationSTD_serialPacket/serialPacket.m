clear;

pcketLens = 9;
packetType = 'int16';

s = kSerial(256000, 'clear');
s.dataBuffer = zeros(pcketLens, 1024 * 8);
s.open();

getFirstSequenceNum = true;
firstSequenceNum = 0;

tic
for i = 1 : 40000
%while true
    [packetData, packetLens] = s.packetRecv(pcketLens, packetType);
    if packetLens > 0
        s.dataBuffer = [s.dataBuffer(:, packetLens + 1 : end), packetData];       % record data
        fprintf('[%5i][%2i]   Gyr[%6i, %6i, %6i]   Acc[%6i, %6i, %6i]   Mag[%6i, %6i, %6i]\n', s.packet.sequenceNum, packetLens, s.dataBuffer(:, end));
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
