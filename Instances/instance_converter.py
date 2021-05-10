import random
import sys
import os

def convert_to__steiner_forest_instance():
    original_instances_dir = sys.argv[1]
    new_instances_dir = sys.argv[2]

    for file_name in os.listdir(original_instances_dir):
        print(file_name)
        path = "{}\\{}".format(original_instances_dir, file_name)

        NODES = 0
        EDGES = ""
        terminals_list = []
        # Reading original file data.
        with open(path) as fp:
            EOF = False
            while not EOF:
                section = False
                split_line = ""
                while not section:
                    read_line = fp.readline()
                    split_line = read_line.split(" ")
                    if read_line.strip() == "EOF":
                        EOF = True
                        break
                    if split_line[0].lower() == "section":
                        section = True

                if not EOF and split_line[1].strip().lower() == "graph":
                    NODES = int(fp.readline().split(" ")[1])
                    edges = int(fp.readline().split(" ")[1])
                    for _ in range(edges):
                        split_line = fp.readline().split(" ")
                        v1 = int(split_line[1]) - 1
                        v2 = int(split_line[2]) - 1
                        w = int(split_line[3])
                        line = f"E {v1} {v2} {2}\n"
                        EDGES += line
                elif not EOF and split_line[1].strip().lower() == "terminals":
                    terminals = int(fp.readline().split(" ")[1])
                    for _ in range(terminals):
                        terminals_list.append(int(fp.readline().split(" ")[1]))
            max_number_of_sets = int(len(terminals_list) / 4)
            number_of_sets = random.randint(2, max_number_of_sets)
            print(f"Number of Sets: {number_of_sets}")
            remainder = len(terminals_list)
            terminals_sets = {}
            for set in range(number_of_sets - 1):
                set_size = random.randint(2, remainder - 2*(number_of_sets - (set + 1)))
                print(f"Set: {set}, Size: {set_size}")
                terminal_set = []
                for _ in range(set_size):
                    terminals_index = random.randint(0, len(terminals_list) - 1)
                    terminal_set.append(terminals_list[terminals_index])
                    terminals_list.remove(terminals_list[terminals_index])
                    remainder = len(terminals_list)
                terminals_sets[set] = terminal_set
            terminals_sets[number_of_sets - 1] = terminals_list



def converter():
    original_instances_dir = sys.argv[1]
    new_instances_dir = sys.argv[2]

    for file_name in os.listdir(original_instances_dir):
        if "_st" in file_name:
            continue
        print(file_name)
        path = "{}\\{}".format(original_instances_dir, file_name)

        data_dict = {}
        # Reading original file data.
        with open(path) as fp:

            split_line = fp.readline().split(" ")
            V = "N {}".format(int(split_line[0]))
            E = int(split_line[1])
            edge_list = []
            for edge in range(E):
                split_line = fp.readline().split(" ")
                vertex_a = int(split_line[0])
                vertex_b = int(split_line[1])
                weight = split_line[2]
                edge_list.append("E {} {} {}".format(vertex_a - 1, vertex_b - 1, weight))

            split_line = fp.readline().split(" ")
            S = int(split_line[0])
            terminal_pair = []
            terminal_groups = []
            if file_name in ["c05.dat", "c07.dat", "c08.dat", "c09.dat", "c10.dat", "c11.dat"]:
                for terminal in range(S):
                    split_line = fp.readline().split(" ")
                    vertex_a = int(split_line[0]) - 1
                    split_line = fp.readline().split(" ")
                    vertex_b = int(split_line[0]) - 1
                    terminal_pair.append((int(vertex_a), int(vertex_b)))
            else:
                for terminal in range(S):
                    split_line = fp.readline().split(" ")
                    terminal_pair.append((int(split_line[0]) - 1, int(split_line[1]) - 1))

            initial_pair = terminal_pair.pop()
            terminal_group = [initial_pair[0], initial_pair[1]]
            terminal_groups.append(terminal_group)
            # joining terminal pair ends
            terminal_group_index = 0
            while len(terminal_pair) > 0:
                added = False
                for p in range(len(terminal_pair)):
                    vertex_a = terminal_pair[p][0]
                    vertex_b = terminal_pair[p][1]
                    if vertex_a in terminal_groups[terminal_group_index]:
                        terminal_pair.pop(p)
                        terminal_groups[terminal_group_index].append(vertex_b)
                        added = True
                        break
                    if vertex_b in terminal_groups[terminal_group_index]:
                        terminal_pair.pop(p)
                        terminal_groups[terminal_group_index].append(vertex_a)
                        added = True
                        break
                if not added:
                    new_group_starting_pair = terminal_pair.pop()
                    terminal_group = [new_group_starting_pair[0], new_group_starting_pair[1]]
                    terminal_groups.append(terminal_group)
                    terminal_group_index += 1

            # writing new instance
            path_new = "{}\\{}".format(new_instances_dir, file_name)
            with open(path_new, "w") as fp2:
                fp2.write("{}\n".format(V))
                for edge in edge_list:
                    fp2.write("{}\n".format(edge))
                for groups in terminal_groups:
                    fp2.write("S ")
                    for terminal in range(len(groups)):
                        if terminal == len(groups) - 1:
                            fp2.write("{}".format(groups[terminal]))
                        else:
                            fp2.write("{} ".format(groups[terminal]))
                    fp2.write("\n")


if __name__ == "__main__":
    convert_to__steiner_forest_instance()
