% 
%       __            ____
%      / /__ _  __   / __/                      __  
%     / //_/(_)/ /_ / /  ___   ____ ___  __ __ / /_ 
%    / ,<  / // __/_\ \ / _ \ / __// _ \/ // // __/ 
%   /_/|_|/_/ \__//___// .__//_/   \___/\_,_/ \__/  
%                     /_/   github.com/KitSprout    
%  
%  @file    kOscilloscope.m
%  @author  KitSprout
%  @date    08-Jan-2018
%  @brief   
% 

classdef kOscilloscope < handle

properties (SetAccess = public)
    curveData;
    curveBufferLens;
    curveChannel;
    curveScale;
    curveColor;
    curveOffset;
    curveMarkerSize;
end

properties (SetAccess = private)
    fig;
    curve;
    window = struct;
    count = 0;
end

methods

    function osc = kSerialOscilloscope( ~ )
        osc.window.width = 800;
        osc.window.displayPos = osc.window.width * (-0.9);
        osc.window.xmax = osc.window.displayPos + osc.window.width;
        osc.window.xmin = osc.window.displayPos;
        osc.window.ymax =  200;
        osc.window.ymin = -200;
    end

	function setWindow( osc, range, width )
        osc.window.width = width;
        osc.window.displayPos = osc.window.width * (-0.9);
        osc.window.xmax = osc.window.displayPos + osc.window.width;
        osc.window.xmin = osc.window.displayPos;
        osc.window.ymax = range(1);
        osc.window.ymin = range(2);
    end

	function initOscilloscope( osc, fig, xName, yName )
        osc.fig = fig;
        grid(osc.fig, 'on');
        hold(osc.fig, 'on');
        xlabel(osc.fig, xName);
        ylabel(osc.fig, yName);
        axis(osc.fig, [osc.window.xmin, osc.window.xmax, osc.window.ymin, osc.window.ymax]);
        for i = 1 : osc.curveChannel
            osc.curve(i) = plot(osc.fig, 0, 0, osc.curveColor{i});
        end
        if isempty(osc.curveScale)
            osc.curveScale = ones(1, osc.curveChannel);
        end
        if isempty(osc.curveOffset)
            osc.curveOffset = zeros(1, osc.curveChannel);
        end
        if isempty(osc.curveMarkerSize)
            osc.curveMarkerSize = 12 * ones(1, osc.curveChannel);
        end
        osc.curveData = zeros(osc.curveChannel, osc.curveBufferLens);
    end

    function updateOscilloscope( osc, data )
        delete(osc.curve);

        lens = size(data, 2);
        osc.count = osc.count + lens;
        osc.curveData = [osc.curveData(:, lens + 1 : end), data];
        runtimes = (osc.count - osc.window.width + 1) : osc.count;

        if osc.count - osc.window.width + 1 > 0
            for i = 1 : osc.curveChannel
                plt = plot(osc.fig, runtimes, osc.curveData(i, end - osc.window.width + 1 : end) .* osc.curveScale(i) + osc.curveOffset(i), osc.curveColor{i});
                plt.MarkerSize = osc.curveMarkerSize(i);
                osc.curve(i) = plt;
            end
        else
            for i = 1 : osc.curveChannel
                plt = plot(osc.fig, runtimes, [zeros(1, osc.window.width - osc.count), osc.curveData(i, end - osc.count + 1 : end)] .* osc.curveScale(i) + osc.curveOffset(i), osc.curveColor{i});
                plt.MarkerSize = osc.curveMarkerSize(i);
                osc.curve(i) = plt;
            end
        end

        osc.window.xmin = osc.window.xmin + lens;
        osc.window.xmax = osc.window.xmin + osc.window.width;
        axis(osc.fig, [osc.window.xmin, osc.window.xmax, osc.window.ymin, osc.window.ymax]);
        drawnow
    end

end

end
