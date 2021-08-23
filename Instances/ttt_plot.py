import sys

import pandas as pd
import plotly.express as px


def ttt_plot(ttt_path, ttt_plot_dir):
    df = pd.read_csv(ttt_path, sep=" ")
    df.columns = ["instance", "finish pos", "iter", "ttt", "cost", "target", "time"]

    unique_instances = df["instance"].unique().tolist()

    for instance in unique_instances:
        real_name = instance.split("/")[-1].split(".")[0]
        target = list(df[df["instance"] == instance]["target"])[0]
        time = list(df[df["instance"] == instance]["time"])[0]
        avg_sol = df[df["instance"] == instance]["cost"].mean()
        times = sorted(df[df["instance"] == instance]["ttt"].to_list())
        n = len(times)
        probabilities = [max((i - 0.5)/n, 0) for i in range(len(times))]
        labels = {
            "x": "Time (s)",
            "y": "Probability"
        }
        title = f"Target: {target}, Avg Cost: {avg_sol:.2f}, Time: {time:.2f}"
        fig = px.scatter(x=times, y=probabilities, labels=labels, title=title)
        fig.add_vline(x=time)
        fig.update_xaxes(range=[0, max(max(times), time) + 0.0001])
        fig.write_image(f"{ttt_plot_dir}\\{real_name}.png")

    print(unique_instances)

def main():
    ttt_path = sys.argv[1]
    ttt_final_path = sys.argv[2]
    print(ttt_path)
    ttt_plot(ttt_path, ttt_final_path)
    print(f"Finished TTT-Ploting. Plotted at: {ttt_final_path}")

if __name__ == "__main__":
    main()
