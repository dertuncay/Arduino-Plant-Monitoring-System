import numpy as np
import itertools
from bokeh.layouts import gridplot
from bokeh.plotting import figure, save, output_file
from bokeh.palettes import Dark2_5 as palette

def smooth(arr, window_size=5):
    """Smooth a 2-D array along the fast axis."""
    
    window = np.ones(window_size) / float(window_size)
    for i in range(arr.shape[1]):
        padded = np.pad(arr[:, i], (window_size, window_size), mode='edge')
        arr[:, i] = np.convolve(padded, window, mode='same')[window_size:-window_size]
    
    return arr

if __name__ == '__main__':
    log = np.loadtxt('log.txt', dtype=float)
    increasing = np.diff(np.concatenate(([0], log[:, 0]))) > 0
    log = log[increasing, :]
    datetime = (log[:, 0] * 1e6).astype(np.datetime64)
    
    window_size = 5
    data = smooth(log[:, 1:], window_size)
    
    output_file("plant_monitor.html", title="Plant monitor dashboard")
    colors = itertools.cycle(palette)

    fig1 = figure(title=f'Moisture sensor values',
                  x_axis_type='datetime')

    for i, name in enumerate(['Avocado', 'Succulent #1', 'Succulent #2', 'Seedlings']):
        fig1.line(datetime, data[:, 3 + i], color=next(colors), legend_label=f'{name}')
    fig1.legend.padding = 2
    fig1.legend.margin = 2

    fig2 = figure(title=f'Temperature',
                  x_range=fig1.x_range,
                  x_axis_type='datetime',
                  y_axis_label='°C')
    fig2.line(datetime, data[:, 0], color=next(colors))

    fig3 = figure(title=f'Relative humidity',
                  x_range=fig1.x_range,
                  x_axis_type='datetime',
                  y_axis_label='%')
    fig3.line(datetime, data[:, 1], color=next(colors))

    fig4 = figure(title=f'Brightness',
                  x_range=fig1.x_range,
                  x_axis_type='datetime',
                  x_axis_label='Time',
                  y_axis_label='/1024')
    fig4.line(datetime, data[:, 2], color=next(colors))

    save(gridplot([[fig1,], [fig2,], [fig3,], [fig4,]], plot_width=400, plot_height=150))  # open a browser
