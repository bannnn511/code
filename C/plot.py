#!/usr/bin/env python3

import matplotlib.pyplot as plt
import numpy as np

# traditional = [0.019966, 0.095837, 0.180244, 0.195340]
# approximate = [0.021546, 0.108778, 0.191885, 0.248192]
approximate = [0.945753, 0.733360, 0.621774, 0.570917, 0.527375,
               0.580986, 0.476303, 0.453609, 0.401342, 0.460146]
a = np.arange(0, 10)
s = 2 ** a

# plt.plot(a, traditional, marker='x')
plt.plot(a, approximate, marker='x')
plt.margins(0)
plt.xticks(a, s)
plt.xlabel('Approximation Factor (S)')
plt.ylabel('Time (seconds)')
# plt.legend(['Precise', 'Approximate'])
plt.savefig('approximate.png', dpi=300)
plt.show()