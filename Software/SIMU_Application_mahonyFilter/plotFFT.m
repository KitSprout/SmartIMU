function f = plotFFT( f, data, freq, state )

if isequal(state, 'init')
    grid(f.fig, 'on');
    hold(f.fig, 'on');
    axis(f.fig, f.window);
    xlabel(f.fig, 'frequence (Hz)');
%    ylabel(cube.fig, '| strength |');
    f.plotBar = plot(0, 0);

else
    data = fft(data);
	P2 = abs(data / f.length);
    P1 = P2(1 : f.length / 2 + 1);
    P1(2 : end - 1) = 2 * P1(2 : end - 1);
    freq = freq * (0 : (f.length / 2)) / f.length;
    delete(f.plotBar);
    f.plotBar = bar(freq, P1, 0.1);
    drawnow

end

end
