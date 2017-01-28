classdef kSerial_Oscilloscope < handle

properties (SetAccess = public)
	curveChannel;
    curveScale;
	curveColor;
end

properties (SetAccess = private)
    fig;
    subPlot;
    curve;
    window = struct;
end

methods

    function osc = kSerial_Oscilloscope( ~ )
        osc.window.width =  800;
        osc.window.displayPos = osc.window.width * (-0.9);
        osc.window.xmax  = osc.window.displayPos + osc.window.width;
        osc.window.xmin  = osc.window.displayPos;
        osc.window.ymax  =  200;
        osc.window.ymin  = -200;
    end

	function setWindow( osc, ymax, ymin, width )
        osc.window.width = width;
        osc.window.displayPos = osc.window.width * (-0.9);
        osc.window.xmax  = osc.window.displayPos + osc.window.width;
        osc.window.xmin  = osc.window.displayPos;
        osc.window.ymax  = ymax;
        osc.window.ymin  = ymin;
    end

	function initOscilloscope( osc, fig )
        osc.fig     = fig;
        osc.subPlot = subplot(1, 1, 1);
        grid(osc.subPlot, 'on');
        hold(osc.subPlot, 'on');
        xlabel(osc.subPlot, 'runtimes');
        ylabel(osc.subPlot, 'data');
        axis(osc.subPlot, [osc.window.xmin, osc.window.xmax, osc.window.ymin, osc.window.ymax]);
        for i = 1 : size(osc.curveChannel, 2)
            osc.curve(i) = plot(osc.subPlot, 0, 0, osc.curveColor(i));
        end
    end

    function updateOscilloscope( osc, s )
        delete(osc.curve);
        runtimes = (s.packet.packetCount - osc.window.width + 1) : s.packet.packetCount;
        for i = 1 : size(osc.curveChannel, 2)
            osc.curve(i) = plot(osc.subPlot, runtimes, s.dataBuffer(osc.curveChannel(i), end - osc.window.width + 1 : end) .* osc.curveScale(i), osc.curveColor(i));
        end
        osc.window.xmin = osc.window.xmin + s.packet.availableData;
        osc.window.xmax = osc.window.xmin + osc.window.width;
        axis(osc.subPlot, [osc.window.xmin, osc.window.xmax, osc.window.ymin, osc.window.ymax]);
        drawnow
    end

end

end
