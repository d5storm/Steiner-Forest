from scipy import stats
import pandas as pd

def clean_name(name):
    split_name = name.split("/")[-1]
    split_name = split_name.split(".")[0]
    return split_name

def wilcoxon(sample_1, sample_2):
    statistic, p_value = stats.wilcoxon(sample_1, sample_2, zero_method="zsplit")
    return p_value

def statistical_eval(df, confidence=0.05):
    instances = df["instance"].unique()
    p_values = {
        "instance": [],
        "p_value": [],
        "relevant": []
    }
    for instance in instances:
        p_values["instance"] += [instance]
        p_value = wilcoxon(df[df["instance"] == instance]["MultiStart"], df[df["instance"] == instance]["MDM"])
        p_values["p_value"] += [p_value]
        p_values["relevant"] += [True if p_value < confidence else False]

    return pd.DataFrame(data=p_values)


def main():
    path = "../results/statistic.csv"
    df = pd.read_csv(path)
    df["instance"] = df["instance"].apply(lambda x: clean_name(x))
    print(statistical_eval(df))



if __name__ == "__main__":
    main()