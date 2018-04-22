clear;

% customizeLens = 3;

s = kSerial(115200, 'clear');
s.setRecordBufferSize(1024 * 16);
% s.setCustomizeBuffer(customizeLens);
s.setRecvThreshold(0);
s.open();

fig = figure(1);
set(fig, 'Position', [100, 200, 1200, 460], 'color', 'w');

subFig(1) = subplot(1, 1, 1);
osc = kSerialOscilloscope();
osc.setWindow(20000 * [1, -1], 400);
osc.curveChannel = 3: 5;
osc.curveColor = {'r', 'g', 'b'};
osc.curveScale = (1) * ones(1, size(osc.curveColor, 2));
osc.initOscilloscope(subFig(1), 'runtime', 'data');

t_idx = 1: 2;
while ishandle(osc.fig)
    [packetData, packetInfo, packetLens] = s.packetRecv();
    if ~isempty(packetLens) && packetLens > 0

        tims = s.ks.data(t_idx, :);
        tt   = s.getTime(t_idx, 0, 0.001);
        freq = s.getFreq(t_idx, 100, 0.001);

        rgyr = s.ks.data( 3:  5, :) / 15;           % +- 2000 dps
        racc = s.ks.data( 6:  8, :) / 7500 * 9.8;   % +- 39.2 m/s^2
        rmag = s.ks.data( 9: 11, :) / 60;           % +- 500 uT
        cgyr = s.ks.data(12: 14, :) / 15;           % +- 2000 dps
        cacc = s.ks.data(15: 17, :) / 7500 * 9.8;   % +- 39.2 m/s^2
        cmag = s.ks.data(18: 20, :) / 60;           % +- 500 uT
        gbia = s.ks.data(21: 23, :) / 200;          % +- 150 dps
        mbia = s.ks.data(24: 26, :) / 200;          % +- 150 uT
        gmag = s.ks.data(27    , :) / 60;           % +- 500 uT
        quat = s.ks.data(28: 31, :) / 10000;        % 
        orie = s.ks.data(32: 36, :) / 80;           % +- 360 deg
        angt = s.ks.data(37: 39, :) / 80;           % +- 360 deg
        gacc = s.ks.data(40: 42, :) / 7500 * 9.8;   % +- 39.2 m/s^2
        lacc = s.ks.data(43: 45, :) / 7500 * 9.8;   % +- 39.2 m/s^2

        fprintf('[%06i][%02i]', s.ks.lens, packetLens);
        fprintf('[%02i:%02i:%02i][%4.0fHz] ', tt(1), tt(2), fix(tt(3) / 10), freq);
%         fprintf('rgyr[%8.2f, %8.2f, %8.2f] ', rgyr(:, end));
%         fprintf('racc[%6.2f, %6.2f, %6.2f] ', racc(:, end));
%         fprintf('rmag[%7.2f, %7.2f, %7.2f] ', rmag(:, end));
%         fprintf('cgyr[%8.2f, %8.2f, %8.2f] ', cgyr(:, end));
%         fprintf('cacc[%6.2f, %6.2f, %6.2f] ', cacc(:, end));
%         fprintf('cmag[%7.2f, %7.2f, %7.2f] ', cmag(:, end));
%         fprintf('gbia[%8.2f, %8.2f, %8.2f] ', gbia(:, end));
%         fprintf('mbia[%7.2f, %7.2f, %7.2f] ', mbia(:, end));
%         fprintf('gmag[%7.2f] ', gmag(:, end));
%         fprintf('quat[%6.3f, %6.3f, %6.3f, %6.3f] ', quat(:, end));
%         fprintf('orie[%7.2f, %7.2f, %7.2f, %7.2f, %7.2f] ', orie(:, end));
        fprintf('angt[%7.2f, %7.2f, %7.2f] ', angt(:, end));
%         fprintf('gacc[%6.2f, %6.2f, %6.2f] ', gacc(:, end));
%         fprintf('lacc[%6.2f, %6.2f, %6.2f] ', lacc(:, end));
        fprintf('\n');

%         pkData = lacc(:, end - packetLens + 1 : end);
%         save_data = [pkData(1, :); pkData(2, :); pkData(3, :)];

%         s.updateCustomizeData(save_data);
        osc.updateOscilloscope(s);
    end
end
s.close();

% {
% check packet
[rate, lost, dt] = s.getLostRate(t_idx, freq, 0.001);
if lost == 0
    fprintf('---- [%05.2f%%] No packet loss ( %i / %i ) ----\n', rate * 100, lost, s.ks.lens);
else
    fprintf('---- [%05.2f%%] Packet loss ( %i / %i ) ----\n', rate * 100, lost, s.ks.lens);
end
% plot(1 : s.ks.lens - 1, dt)
%}
% {
s.save2mat('log/rawData', {'tims(1:2)'});
%}
