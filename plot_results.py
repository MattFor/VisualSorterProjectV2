#!/usr/bin/env python3

#
# Created by MattFor on 01.04.2025.
#

import os
import sys
import pandas as pd
import matplotlib.pyplot as plt

# Determine which CSV file to use
multi_csv = "../results/performance_results_multi.txt"
default_csv = "../results/performance_results.txt"

if os.path.exists(multi_csv):
	answer = input("Multi-size results file detected. Would you like to use it for plotting? (y/n): ")
	csv_file = multi_csv if answer.lower() == "y" else default_csv
else:
	csv_file = default_csv

# Read the CSV into a DataFrame
try:
	df = pd.read_csv(csv_file)
except Exception as e:
	print(f"Error reading {csv_file}: {e}")
	sys.exit(1)

desired_tests = ["Mixed", "All Negative", "All Positive", "Already Sorted", "Reverse Sorted"]
df = df[df["TestType"].isin(desired_tests)]
if df.empty:
	print("No data found for the desired test types. Exiting.")
	sys.exit(1)

df["ArraySize"] = pd.to_numeric(df["ArraySize"], errors="coerce")
df = df.dropna(subset=["ArraySize"])

group1 = [
	"Bubble Sort",
	"Insertion Sort",
	"Selection Sort",
	"Cocktail Sort",
	"Gnome Sort"
]

group2 = [
	"Quicksort",
	"Shell Sort",
	"Heap Sort",
	"Radix LSD Bucket Sort",
	"Timsort",
	"Spreadsort"
]

combined = group1 + group2

output_dir = "../results/plots"
os.makedirs(output_dir, exist_ok=True)
os.makedirs(output_dir + "/multi", exist_ok=True)
os.makedirs(output_dir + "/single", exist_ok=True)

multi_mode = (csv_file == multi_csv)

total_graphs = len(desired_tests) * 3
graph_counter = 0


# Function for line plots (used in multi-size mode)
def plot_line(df_group, title, output_filename):
	plt.figure(figsize=(16, 9))  # Wider figure.

	for alg in df_group["Algorithm"].unique():
		df_alg = df_group[df_group["Algorithm"] == alg]
		df_alg = df_alg.sort_values("ArraySize")
		plt.plot(df_alg["ArraySize"], df_alg["Duration_ms"], marker="o", label=alg)

	plt.xlabel("Array Size", fontsize=14)
	plt.ylabel("Duration (ms)", fontsize=14)
	plt.title(title, fontsize=16)
	plt.legend(loc="best", fontsize=12)
	plt.grid(True)
	plt.tight_layout()
	plt.savefig(os.path.join(output_dir + "/multi", output_filename))
	plt.close()


# Function for bar plots (used in single-size mode)
def plot_bar(df_group, title, output_filename):
	plt.figure(figsize=(14, 8))
	# For each algorithm, get the average duration
	algs = df_group["Algorithm"].unique()
	durations = []

	for alg in algs:
		duration = df_group[df_group["Algorithm"] == alg]["Duration_ms"].mean()
		durations.append(duration)

	x = range(len(algs))
	plt.bar(x, durations, tick_label=algs)
	plt.xlabel("Algorithm", fontsize=14)
	plt.xticks(rotation=45, ha="right")
	plt.ylabel("Duration (ms)", fontsize=14)
	plt.title(title, fontsize=16)
	plt.grid(True)
	plt.tight_layout()
	plt.savefig(os.path.join(output_dir + "/single", output_filename))
	plt.close()


# Loop through each test type.
for test_type in desired_tests:
	df_test = df[df["TestType"] == test_type]
	if df_test.empty:
		print(f"No data for test type '{test_type}' found. Skipping.")
		continue

	# Graph 1: Group I only
	graph_counter += 1
	title1 = f"{test_type} Data: Basic Methods (Group I)"
	filename1 = f"{test_type}_group1.png" if not multi_mode else f"{test_type}_group1_line.png"
	print(f"Generating graph {graph_counter} of {total_graphs}: {title1}")
	df_group1 = df_test[df_test["Algorithm"].isin(group1)]
	if multi_mode:
		plot_line(df_group1, title1, filename1)
	else:
		plot_bar(df_group1, title1, filename1)

	# Graph 2: Group II only
	graph_counter += 1
	title2 = f"{test_type} Data: Advanced Methods (Group II)"
	filename2 = f"{test_type}_group2.png" if not multi_mode else f"{test_type}_group2_line.png"
	print(f"Generating graph {graph_counter} of {total_graphs}: {title2}")
	df_group2 = df_test[df_test["Algorithm"].isin(group2)]
	if multi_mode:
		plot_line(df_group2, title2, filename2)
	else:
		plot_bar(df_group2, title2, filename2)

	# Graph 3: Combined groups
	graph_counter += 1
	title3 = f"{test_type} Data: Combined Basic and Advanced Methods"
	filename3 = f"{test_type}_combined.png" if not multi_mode else f"{test_type}_combined_line.png"
	print(f"Generating graph {graph_counter} of {total_graphs}: {title3}")
	df_combined = df_test[df_test["Algorithm"].isin(combined)]
	if multi_mode:
		plot_line(df_combined, title3, filename3)
	else:
		plot_bar(df_combined, title3, filename3)

print("Plotting complete. Plots saved to:", output_dir)
print(f"{graph_counter} graphs were generated out of a total of {total_graphs} expected.")

answer = input("Would you like to display the graphs now? (y/n): ")
if answer.lower() == "y":
	for file in os.listdir(output_dir):
		filepath = os.path.join(output_dir, file)
		if sys.platform.startswith("win"):
			os.startfile(filepath)
		elif sys.platform.startswith("linux"):
			os.system(f"xdg-open \"{filepath}\"")
		elif sys.platform.startswith("darwin"):  # MacOS
			os.system(f"open \"{filepath}\"")
		else:
			print(f"Please open the file manually: {filepath}")
else:
	print("Graphs will not be displayed.")

input("Press Enter to close graphing program...")
