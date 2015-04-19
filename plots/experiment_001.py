#!/usr/bin/python

import matplotlib.pyplot as plt
import numpy as np
from matplotlib.patches import Rectangle

plt.figure(figsize=(6.5, 4))
plt.subplots_adjust(left=0.11, right=0.97, top=0.97, bottom=0.1)

#plt.subplot(1, 2, 1);
plt.xticks([0, 0.5, 1, 1.5])
data1 = np.genfromtxt('results/vi_001.data.gz', delimiter=' ', names=['i_dc', 'v_av'])

plot_nav_is_1 = plt.plot(data1['i_dc'], data1['v_av'], label='$N_{av}=1$')
plt.setp(plot_nav_is_1, color='black', linewidth=3.0)

data2 = np.genfromtxt('results/vi_002.data.gz', delimiter=' ', names=['i_dc', 'v_av'])

plot_nav_is_2 = plt.plot(data2['i_dc'], data2['v_av'], label='$N_{av}=3$')
plt.setp(plot_nav_is_2, color='r', linewidth=1.0)

plt.xlabel('$i_{dc}$', fontsize=14, labelpad=-5)
plt.ylabel('$\overline{v}$', fontsize=14)

plot_diff = plt.plot(data2['i_dc'], data2['v_av']-data1['v_av'], label='diff')
plt.setp(plot_diff, color='blue', linewidth=1.0)

currentAxis = plt.gca()
currentAxis.text(1, -0.1, '$a$', fontsize=15)
currentAxis.text(1.28, -0.1, '$b$', fontsize=15)

currentAxis.add_patch(Rectangle((0.915, -0.2), 0.193, 1.2, facecolor=(0.95, 0.95, 0.95)))
currentAxis.add_patch(Rectangle((1.25, -0.2), 0.1, 1.2, facecolor=(0.95, 0.95, 0.95)))


plt.legend(loc='upper left')
outfile = 'docs/figures/experiments_001_and_002.pdf'
print("Writing " + outfile)
plt.savefig(outfile)
plt.show()
