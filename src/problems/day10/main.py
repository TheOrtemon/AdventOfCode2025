import pathlib

from z3 import Int, Optimize, sat

aoc_dir = pathlib.Path(__file__).parent.parent.parent.parent
file_path = aoc_dir / "inputs" / "day10.txt"

with open(file_path, "r") as file:
    content = file.read()

test_data = """[.##.] (3) (1,3) (2) (2,3) (0,2) (0,1) {3,5,4,7}
[...#.] (0,2,3,4) (2,3) (0,4) (0,1,2) (1,2,3,4) {7,5,12,7,2}
[.###.#] (0,1,2,3,4) (0,3,4) (0,1,2,4,5) (1,2) {10,11,11,5,10,5}"""
test_result = 33

input_data = content


def solve_line(buttons: list[list[int]], joules: list[int]) -> list[int]:
    but_len = len(buttons)
    joules_len = len(joules)
    opt = Optimize()
    matrix = [[0] * but_len for _ in range(joules_len)]

    for i in range(but_len):
        button = buttons[i]
        for but_part in button:
            matrix[but_part][i] = 1

    presses = [Int(f"n_{i}") for i in range(but_len)]
    for i in range(but_len):
        opt.add(presses[i] >= 0)

    for i in range(joules_len):
        opt.add(sum(matrix[i][j] * presses[j] for j in range(but_len)) == joules[i])

    opt.minimize(sum(presses))
    if opt.check() != sat:
        print("Error!!!")
        return []
    mod = opt.model()
    solution = [mod[presses[i]].as_long() for i in range(but_len)]

    return solution


def main():
    res = 0
    for line in input_data.splitlines():
        parts = [
            [int(n) for n in part[1:-1].split(",")] for part in line.split(" ")[1:]
        ]
        buttons = parts[:-1]
        joules = parts[-1]
        res += sum(solve_line(buttons, joules))

    print("Hello from day10!")
    print(f"{res=}")


if __name__ == "__main__":
    main()
