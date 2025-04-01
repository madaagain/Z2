from typing import Any, Self


class ImpossibleError(Exception):
    "This shows the code accessing a line it shouldn't be able to"
    def __init__(self: 'ImpossibleError', *args: object) -> None:
        super().__init__(*args)


class SizeError(Exception):
    """This shows a setting size depending between two things that aren't the same size failing"""
    def __init__(self: 'SizeError', *args: object) -> None:
        super().__init__(*args)


class Matrix:
    data: list[Any]
    _x: int
    _y: int

    def __init__(self: 'Matrix', x: int = 1, y: int = 1, *args: Any) -> None:
        self._x = x
        self._y = y
        nb = 0
        self.data = []
        for i in range(len(args)):
            try:
                self.data = self.data + args[i]
                nb += len(args[i])
            except TypeError:
                self.data = self.data + [args[i]]
                nb += 1
        while nb < x * y:
            test = False
            if self._x == self._y and not args:
                for i in range(x):
                    if i * x + i == nb:
                        self.data = self.data + [1]
                        nb += 1
                        test = True
            if not test:
                self.data = self.data + [0]
                nb += 1
        return

    def get_x(self: Self) -> int:
        return self._x

    def get_y(self: Self) -> int:
        return self._y

    def get_value(self: Self, x: int, y: int) -> int | float:
        return (self.data[x * self._y + y])

    def __add__(self: Self, other: Any) -> 'Matrix':
        data = self.data.copy()
        if isinstance(type(other), type(self)):
            if not (self._x == other.x and self._y == other.y):
                raise SizeError(
                    f"The {type(self)} cannot be added to each other")
            for i in range(self._x):
                for j in range(self._y):
                    data[i * self._y + j] = self.get_value(
                        i, j) + other.get_value(i, j)
            return Matrix(self._x, self._y, data)
        if isinstance(type(other), int) or isinstance(type(other), float):
            for i in range(len(self.data)):
                data[i] += other
            return Matrix(self._x, self._y, data)
        raise (NotImplementedError(f"{other} is off wrong type"))

    def __sub__(self: Self, matrice: 'Matrix') -> 'Matrix':
        data = self.data.copy()
        if not (self._x == matrice._x and self._y == matrice._y):
            raise SizeError(
                f"The {type(self)} cannot be subbed from each other")
        for i in range(self._x):
            for j in range(self._y):
                data[i * self._y + j] = self.get_value(
                    i, j) - matrice.get_value(i, j)
        return Matrix(self._x, self._y, data)

    def __mul__(self: Self, other: Any) -> 'Matrix':
        if isinstance(type(other), int) or isinstance(type(other), float):
            data = self.data.copy()
            for i in range(len(data)):
                data[i] = data[i] * other
            return Matrix(self._x, self._y, data)

        if isinstance(type(other), type(self)):
            data: list[Any] = []
            if not self._y == other.x:
                raise SizeError(
                    f"The {type(self)} cannot multiply with each other")
            for i in range(self._x):
                for j in range(other.y):
                    value = type(self.get_value(i, 0))()
                    for k in range(self._y):
                        value = value + self.get_value(
                            i, k) * other.get_value(k, j)
                    data += [value]
            return Matrix(self._x, other.y, data)
        raise TypeError(
            f"{type(self)} cannot be multiplied with {type(other)}")

    def __str__(self: Self) -> str:
        value: list[Any] = []
        string = ""
        for i in range(self._x):
            for j in range(self._y):
                value = value + [self.get_value(i, j)]
            string = string + str(value)
            value = []
            if i + 1 != self._x:
                string = string + "\n"
        return string

    def __repr__(self: Self) -> str:
        string = ""
        for i in range(self._x):
            for j in range(self._y):
                if (self.get_value(i, j) == 0.0):
                    string = string + "0.00"
                else:
                    string = string + f"{self.get_value(i, j):.2f}"
                if (j != range(self._y)[-1]):
                    string += "\t"
            string = string + "\n"
        return string

    def transpose(self: Self) -> 'Matrix':
        data: list[Any] = []
        for j in range(self._y):
            for i in range(self._x):
                data.append(self.get_value(i, j))
        return Matrix(self._y, self._x, data)

    def __get_cofactor__(self: Self, i: int, j: int) -> int | float:
        if self._x != self._y:
            raise SizeError(f"The {type(self)} isn't a square")
        if self._x < 3:
            raise ValueError(
                f"Can't operate cofactors on any smaller {type(self)}")
        matrice = Matrix(
            self._x - 1, self._y - 1,
            [0 for _ in range((self._x - 1) * (self._y - 1))])
        matrice.data = []
        for x in range(self._x):
            for y in range(self._y):
                if i - 1 != x and j - 1 != y:
                    matrice.data.append(self.get_value(x, y))
        return (-1)**(i + j) * matrice.det()

    def det(self: Self) -> int | float:
        if self._x != self._y:
            raise SizeError(f"The {type(self)} isn't a square")
        if self._x == 1:
            return self.data[0]
        if self._x == 2:
            return self.get_value(0, 0) * self.get_value(
              1, 1) - self.get_value(0, 1) * self.get_value(1, 0)
        if self._x > 2:
            value = 0
            for i in range(self._x):
                value += self.get_value(i, 0) * self.__get_cofactor__(
                    i + 1, 1)
            return value
        raise ImpossibleError()

    def inv(self: Self) -> 'Matrix':
        if self._x != self._y:
            raise SizeError(f"The {type(self)} isn't a square")
        if self.det() == 0:
            raise ValueError(f"The {type(self)} determinant is 0")
        if self._x == 1:
            return Matrix(1, 1, 1 / self.data[0])
        if self._x == 2:
            return Matrix(2, 2, self.get_value(1, 1), -self.get_value(0, 1),
                          -self.get_value(1, 0), self.get_value(0, 0)) * (1 / self.det())
        if self._x > 2:
            data: list[Any] = []
            for i in range(self._x):
                for j in range(self._y):
                    data.append(self.transpose().__get_cofactor__(
                        i + 1, j + 1))
            return Matrix(self._x, self._y, data) * (1/self.det())
        raise ImpossibleError()

    def __eq__(self: Self, __o: Any) -> bool:
        if isinstance(__o, Matrix):
            return (self._x == __o._x) and (self._y == __o._y) and (
                self.data == __o.data)
        try:
            iter(__o)
            return len(__o) == self._x * self._y and all(
                [__o[i] == self.data[i] for i in range(len(__o))])
        except TypeError:
            pass
        raise NotImplementedError(
            f"Can't compare {type(self)} and {type(__o)}")

    def copy(self: Self) -> 'Matrix':
        return Matrix(self._x, self._y, self.data)

    def __truediv__(self: Self, other: Any) -> 'Matrix':
        data = self.data.copy()
        if isinstance(other, int) or isinstance(other, float):
            x = self.get_x()
            for i in range(self.get_y()):
                for j in range(x):
                    data[i * x + j] = data[i * x + j] / other
            return Matrix(self._x, self._y, data)
        raise (NotImplementedError(
            f"Cannot divide {type(self)} with {type(other)}!"))

    def __call__(self: Self, x: int, y: int) -> Any:
        return self.get_value(x, y)

    def set_value(self: Self, x: int, y: int, value: int | float) -> None:
        if x >= self._x or y >= self._y:
            raise IndexError()
        self.data[x * self._y + y] = value

    def __del__(self: Self):
        del self.data
