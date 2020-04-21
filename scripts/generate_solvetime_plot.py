import csv
from matplotlib import pyplot as plt


class ProblemSolution:
    def __init__(self, name, num_nodes, solution_time, upper_bound, prize):
        self.name = name
        self.num_nodes = num_nodes
        self.solution_time = solution_time
        self.upper_bound = upper_bound
        self.prize = prize
        self.ratio = upper_bound / prize  # Should be less than 2
        # Technically the guarantee is 2 + epsilon, so a smidge over is ok.
        if (self.ratio > 2.0):
            print("Warning: Problem {} has ratio {}, should be less than 2 + epsilon".format(
                name, self.ratio))

problem_solutions = []

def load():
    with open("solve_times.csv") as csvfile:
        first_row = True
        for row in csv.reader(csvfile, delimiter=","):
            if (first_row):
                first_row = False
                continue
            if int(row[4]) != 1:
                continue
            problem_solutions.append(
                ProblemSolution(name=row[5],
                                num_nodes=int(row[0]),
                                solution_time=float(row[1]),
                                upper_bound=float(row[2]),
                                prize=float(row[3])))


def plot_solution_times():
    num_nodes = [prob.num_nodes for prob in problem_solutions]
    solution_times = [prob.solution_time for prob in problem_solutions]
    plt.semilogy(num_nodes, solution_times, marker=".", linestyle="")
    plt.xlabel("Number of nodes")
    plt.ylabel("Solve time (seconds)")
    plt.title("Solve time versus problem size")
    plt.show()


def plot_approximation_ratios():
    num_nodes = [prob.num_nodes for prob in problem_solutions]
    ratios = [prob.ratio for prob in problem_solutions]
    plt.plot(num_nodes, ratios)
    plt.xlabel("Number of nodes")
    plt.ylabel("Ratio (upper/lower)")
    plt.title("Approximation ratio versus problem size")
    plt.axhline(2)
    plt.show()


load()
plot_solution_times()
#plot_approximation_ratios()
