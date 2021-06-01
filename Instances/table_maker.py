import sys


def main():
    raw_result = sys.argv[1]
    final_file = sys.argv[2]

    with open(raw_result) as fp:
        lines = fp.readlines()

    instances_dict = {}

    for line in lines:
        split_line = line.split(" ")
        name = split_line[0].strip().split("/")[-1].split(".")[0]
        value = float(split_line[2].strip())
        time = float(split_line[3].strip())
        if name in instances_dict:
            instances_dict[name]["avg"] += value
            instances_dict[name]["time"] += time
            instances_dict[name]["occurrence"] += 1
            if value < instances_dict[name]["best"]:
                instances_dict[name]["best"] = value
        else:
            instances_dict[name] = {
                "avg": value,
                "best": value,
                "occurrence": 1,
                "time": time
            }

    with open(final_file, "w") as fp:
        fp.write("Instância,best,media,tempo\n")
        for file, values in instances_dict.items():
            fp.write(f"{file},{values['best']},{(values['avg'] / values['occurrence']):.2f},"
                     f"{(values['time'] / values['occurrence']):.2f}\n")


if __name__ == "__main__":
    main()
