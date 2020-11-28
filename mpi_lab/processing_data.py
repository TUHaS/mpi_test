from pathlib import Path

import seaborn as sns
import pandas as pd
import matplotlib.pyplot as plt

if __name__ == '__main__':
    BASE_DIR = Path(__file__).resolve(strict=True).parent
    data_path = BASE_DIR / 'full_data.csv'
    plt.rcParams['figure.figsize'] = (25, 9)
    data_frame = pd.read_csv(data_path, sep=",", encoding="utf-8")
    sns.set_theme(style="darkgrid")
    for i in range(2, 25):
        current_processors = data_frame[data_frame["processors_count"] == i]
        sns.lineplot(data=current_processors, x="block_size", y="time", hue="processors_count")
        plt.savefig(f'procs_num_{i}.png')
        plt.close()