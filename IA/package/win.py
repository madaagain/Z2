import itertools
import tkinter as tk
import colorsys


NB_COL: int = int(1e3)

# Create the main window
root: tk.Tk = tk.Tk()
root.title("Victory!")
root.geometry("600x200")
root.configure(bg='#ffffff')


def change_background_color():
    hue: float = next(colors_cycle)
    new_col: tuple[float, float, float] = colorsys.hsv_to_rgb(hue, 1.0, 1.0)
    new_color: str = "#{0:02x}{1:02x}{2:02x}".format(*map(lambda x: int(x * 255), new_col))
    root.configure(bg=new_color)
    victory.configure(bg=new_color)
    root.after(10, change_background_color)


colors: list[float] = [1.0 * float(i) / NB_COL for i in range(NB_COL)]
colors_cycle = itertools.cycle(colors)
victory: tk.Label = tk.Label(root, text="Congratulations!\nThis AI won the game!!!", font=("Arial", 40), fg="#000000")
victory.pack()
root.after(10, change_background_color)
root.mainloop()
exit()
