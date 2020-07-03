import sys
import os


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

converter()
