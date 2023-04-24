from time import perf_counter
from functools import wraps

"""A decorator that reports the execution time."""


def clock(func):
    @wraps(func)
    def clocked(*args, **kwargs):
        t0 = perf_counter()
        result = func(*args, **kwargs)
        elapsed = perf_counter() - t0
        name = func.__name__
        arg_lst = [repr(arg) for arg in args]
        arg_lst.extend(f"{k}={v!r}" for k, v in kwargs.items())
        arg_str = ", ".join(arg_lst)

        output = f"[{elapsed:0.8f}s] {name}({arg_str}) -> {result!r}"
        fill = "="
        fill_len = len(output)
        print(f'{"CLOCK START":{fill}^{fill_len}}')
        print(output)
        print(f'{"CLOCK ENDED":{fill}^{fill_len}}')

        return result

    return clocked
