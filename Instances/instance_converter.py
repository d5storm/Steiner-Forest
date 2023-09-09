import copy
import random
import sys
import os

random.seed(0)


def ghalami_to_luidi():
    original_instances_dir = sys.argv[1]
    new_instances_dir = sys.argv[2]

    for file_name in os.listdir(original_instances_dir):
        print(file_name)
        path = "{}/{}".format(original_instances_dir, file_name)

        NODES = 0
        EDGES = ""
        terminals_list = ""
        # Reading original file data.
        with open(path) as fp:
            terminals_read = False
            EOF = False
            while not EOF:
                section = False
                split_line = ""
                while not section:
                    read_line = fp.readline()
                    split_line = read_line.split(" ")
                    if read_line.strip() == "END" and terminals_read:
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
                        line = f"E {v1} {v2} {w}\n"
                        EDGES += line
                elif not EOF and split_line[1].strip().lower() == "terminals":
                    terminals_read = True
                    S = int(int(fp.readline().strip().split(" ")[1]) / 2)
                    terminal_pair = []
                    terminal_groups = []
                    for _ in range(S):
                        split_terminals = fp.readline().strip().split(" ")
                        terminal_a = int(split_terminals[1]) - 1
                        terminal_b = int(split_terminals[2]) - 1
                        terminal_pair.append((terminal_a, terminal_b))
                        terminals_list += f"S {terminal_a} {terminal_b}\n"

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

        new_file_name = f"{file_name.split('.')[0]}_sft.{file_name.split('.')[1]}"

        path = "{}/{}".format(new_instances_dir, new_file_name)

        with open(path, "w") as fp2:
            fp2.write(f"N {NODES}\n")
            fp2.write(EDGES)
            for groups in terminal_groups:
                fp2.write("S ")
                for terminal in range(len(groups)):
                    if terminal == len(groups) - 1:
                        fp2.write("{}".format(groups[terminal]))
                    else:
                        fp2.write("{} ".format(groups[terminal]))
                fp2.write("\n")


def create_random_terminal_sets(terminals_list, tree=False):
    max_number_of_sets = int(len(terminals_list) / 4)
    if max_number_of_sets < 2 or tree:
        new_terminals = [id - 1 for id in terminals_list]
        return {0: new_terminals}
    else:
        number_of_sets = random.randint(2, max_number_of_sets)
        remainder = len(terminals_list)
        terminals_sets = {}
        for set in range(number_of_sets - 1):
            set_size = random.randint(2, remainder - 2 * (number_of_sets - (set + 1)))
            terminal_set = []
            for _ in range(set_size):
                terminals_index = random.randint(0, len(terminals_list) - 1)
                terminal_set.append(terminals_list[terminals_index] - 1)
                terminals_list.remove(terminals_list[terminals_index])
                remainder = len(terminals_list)
            terminals_sets[set] = terminal_set
        terminals_sets[number_of_sets - 1] = terminals_list

        return terminals_sets


def create_luidi_terminal_sets(terminals_list):
    even_pairs = []
    uneven_pairs = []
    roundrobin_pairs = []
    for index in range(0, len(terminals_list), 4):
        if index + 2 < len(terminals_list):
            pos_a = terminals_list[index]
            pos_b = terminals_list[index + 2]

            if pos_a > pos_b:
                even_pairs.append((pos_b, pos_a))
            else:
                even_pairs.append((pos_a, pos_b))

    for index in range(1, len(terminals_list), 4):
        if index + 2 < len(terminals_list):
            pos_a = terminals_list[index]
            pos_b = terminals_list[index + 2]

            if pos_a > pos_b:
                uneven_pairs.append((pos_b, pos_a))
            else:
                uneven_pairs.append((pos_a, pos_b))

    index_2 = len(terminals_list) - 1
    for index in range(0, int(len(terminals_list) / 2)):
        pos_a = terminals_list[index]
        pos_b = terminals_list[index_2]
        if pos_a > pos_b:
            roundrobin_pairs.append((pos_b, pos_a))
        else:
            roundrobin_pairs.append((pos_a, pos_b))
        index_2 -= 1

    terminals_set = {}
    terminal_count = 0
    used_list = []
    while len(even_pairs) + len(uneven_pairs) + len(roundrobin_pairs) > 0:
        if len(roundrobin_pairs) > 0:
            start = roundrobin_pairs.pop(0)
            check_list = [start[0], start[1]]
        elif len(even_pairs) > 0:
            start = even_pairs.pop(0)
            check_list = [start[0], start[1]]
        else:
            start = uneven_pairs.pop(0)
            check_list = [start[0], start[1]]
        terminal_list = []
        while len(check_list) > 0:
            terminal = check_list.pop(0)
            terminal_list.append(terminal - 1)
            used_list.append(terminal)
            for_limit = len(roundrobin_pairs)
            index = 0
            while index < for_limit:
                pair = roundrobin_pairs[index]
                if pair[0] == terminal or pair[1] == terminal:
                    roundrobin_pairs.pop(index)
                    for_limit -= 1
                    if pair[0] not in check_list and pair[0] not in used_list:
                        check_list.append(pair[0])
                    if pair[1] not in check_list and pair[1] not in used_list:
                        check_list.append(pair[1])
                index += 1
            for_limit = len(even_pairs)
            index = 0
            while index < for_limit:
                pair = even_pairs[index]
                if pair[0] == terminal or pair[1] == terminal:
                    even_pairs.pop(index)
                    for_limit -= 1
                    if pair[0] not in check_list and pair[0] not in used_list:
                        check_list.append(pair[0])
                    if pair[1] not in check_list and pair[1] not in used_list:
                        check_list.append(pair[1])
                index += 1
            for_limit = len(uneven_pairs)
            index = 0
            while index < for_limit:
                pair = uneven_pairs[index]
                if pair[0] == terminal or pair[1] == terminal:
                    uneven_pairs.pop(index)
                    for_limit -= 1
                    if pair[0] not in check_list and pair[0] not in used_list:
                        check_list.append(pair[0])
                    if pair[1] not in check_list and pair[1] not in used_list:
                        check_list.append(pair[1])
                index += 1
        terminals_set[terminal_count] = terminal_list

        terminal_count += 1
    return terminals_set


def convert_to__steiner_forest_instance():
    original_instances_dir = sys.argv[1]
    new_instances_dir = sys.argv[2]

    for file_name in os.listdir(original_instances_dir):
        print(file_name)
        path = "{}\\{}".format(original_instances_dir, file_name)

        NODES = 0
        EDGES = ""
        terminals_list = []
        random_terminals = None
        luidi_terminals = None
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
                        line = f"E {v1} {v2} {w}\n"
                        EDGES += line
                elif not EOF and split_line[1].strip().lower() == "terminals":
                    terminals = int(fp.readline().split(" ")[1])
                    for _ in range(terminals):
                        terminals_list.append(int(fp.readline().split(" ")[1]))
            random_terminals = create_random_terminal_sets(copy.deepcopy(terminals_list))
            tree_terminals = create_random_terminal_sets(copy.deepcopy(terminals_list), tree=True)
            luidi_terminals = create_luidi_terminal_sets(copy.deepcopy(terminals_list))

        new_file_name = f"{file_name.split('.')[0]}_sft.{file_name.split('.')[1]}"

        path = "{}\\{}".format(new_instances_dir, new_file_name)

        with open(path, "w") as fp2:
            fp2.write(f"N {NODES}\n")
            fp2.write(EDGES)
            for set, terminals in tree_terminals.items():
                fp2.write("S ")
                for index in range(len(terminals) - 1):
                    id = terminals[index]
                    fp2.write(f"{id} ")
                fp2.write(f"{terminals[-1]}\n")

        new_file_name = f"{file_name.split('.')[0]}_sf_r.{file_name.split('.')[1]}"

        path = "{}\\{}".format(new_instances_dir, new_file_name)

        with open(path, "w") as fp2:
            fp2.write(f"N {NODES}\n")
            fp2.write(EDGES)
            for set, terminals in random_terminals.items():
                fp2.write("S ")
                for index in range(len(terminals) - 1):
                    id = terminals[index]
                    fp2.write(f"{id} ")
                fp2.write(f"{terminals[-1]}\n")

        new_file_name = f"{file_name.split('.')[0]}_sf.{file_name.split('.')[1]}"

        path = "{}\\{}".format(new_instances_dir, new_file_name)

        with open(path, "w") as fp2:
            fp2.write(f"N {NODES}\n")
            fp2.write(EDGES)
            for set, terminals in luidi_terminals.items():
                fp2.write("S ")
                for index in range(len(terminals) - 1):
                    id = terminals[index]
                    fp2.write(f"{id} ")
                fp2.write(f"{terminals[-1]}\n")


def convert_to_luidi(linux=False):
    separator = "\\"
    if linux:
        separator = "/"
    original_instances_dir = sys.argv[1]
    new_instances_dir = sys.argv[2]

    for file_name in os.listdir(original_instances_dir):
        path = "{}{}{}".format(original_instances_dir, separator, file_name)
        with open(path) as fp:
            N = fp.readline().split(" ")[1].strip()
            E_number = 0
            E = ""
            S = []
            EOF = False
            while not EOF:
                read_line = fp.readline()
                if read_line.strip() == "":
                    EOF = True
                    break
                split_line = read_line.split(" ")
                if split_line[0] == "E":
                    E_number += 1
                    E += f"{int(split_line[1]) + 1} {int(split_line[2]) + 1} {split_line[3].strip()} 1\n"
                else:
                    for i in range(1, len(split_line) - 1):
                        S.append(f"{int(split_line[i]) + 1} {int(split_line[i + 1].strip()) + 1}")

            path_new = "{}{}{}".format(new_instances_dir, separator, file_name)
            with open(path_new, "w") as fp2:
                fp2.write(f"{N} {E_number}\n")
                fp2.write(E)
                fp2.write(f"{len(S)}\n")
                for x in S:
                    fp2.write(f"{x}\n")
                fp2.write("6\n1\n13\n499")


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
    convert_to_luidi(linux=True)
