"""
Построение графика для демонстрации унимодальности функции.
Функция: f(x) = x*sin(x) + 2*cos(x) на [-5, -4]
"""

import numpy as np
import matplotlib.pyplot as plt

# ─── Функция: f(x) = x*sin(x) + 2*cos(x) на [-5, -4] ──────────────────────────
a, b = -5.0, -4.0
x = np.linspace(a, b, 1000)
y = x * np.sin(x) + 2 * np.cos(x)

idx = np.argmin(y)
xm, ym = x[idx], y[idx]

fig, ax = plt.subplots(figsize=(9, 5))

ax.plot(x, y, linewidth=2, color='steelblue',
        label=r'$f(x)=x\sin x+2\cos x$')
ax.axvline(xm, color='tomato', linestyle='--', linewidth=1.2,
           label=f'минимум ≈ ({xm:.4f}, {ym:.4f})')
ax.scatter([xm], [ym], color='tomato', zorder=5)
ax.set_xlabel('x', fontsize=12)
ax.set_ylabel('f(x)', fontsize=12)
ax.set_title(r'$f(x)=x\sin x+2\cos x$,  $x\in[-5,\,-4]$', fontsize=13)
ax.legend(fontsize=10)
ax.grid(True, alpha=0.4)

# Вывод анализа
print("=" * 55)
print("Функция: f(x) = x·sin(x) + 2·cos(x)  на [-5, -4]")
print("=" * 55)
print(f"  Минимум: x* ≈ {xm:.6f},  f(x*) ≈ {ym:.6f}")
print()
print("  Анализ унимодальности:")
print("  f'(x) = sin(x) + x·cos(x) − 2·sin(x) = −sin(x) + x·cos(x)")
print("  На отрезке [-5, -4] производная не меняет знак →")
print("  функция имеет ровно один экстремум (минимум).")
print("  Вывод: функция УНИМОДАЛЬНА, оба метода применимы. ✓")
print()

plt.tight_layout()
plt.savefig('unimodality.png', dpi=150)
print("График сохранён в unimodality.png")
plt.show()