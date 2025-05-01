import subprocess
import time
import os

# mutants = ["sort_mutant_1.h", "sort_mutant_2.h", "sort_mutant_3.h", "sort_mutant_4.h", "sort_mutant_5.h", "sort_mutant_6.h", "sort_mutant_7.h", "sort_mutant_8.h"] 
mutants = ["sort_mutant_1.h"]

def run_mutant(mutant_file):
    print(f"Тестирую: {mutant_file}")

    with open("sort_current.h", "w") as dest:
        with open(f"{mutant_file}", "r") as src:
            dest.write(src.read())

    start = time.time()

    # Скомпилируй
    try:
        subprocess.run(
            ["g++", "framework_rapidcheck.cpp", "-std=c++17", "-o", "test_exec"],
            check=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE
        )
    except subprocess.CalledProcessError as e:
        print("Ошибка компиляции:", e.stderr.decode())
        return "compile_error", 0.0

    # Запусти
    try:
        result = subprocess.run(["./test_exec"], timeout=10)
        end = time.time()
        if result.returncode != 0:
            return "killed", end - start
        else:
            return "survived", end - start
    except subprocess.TimeoutExpired:
        return "timeout", 10.0

def main():
    summary = {"killed": 0, "survived": 0, "timeout": 0, "compile_error": 0}
    total_time = 0

    for mutant in mutants:
        status, duration = run_mutant(mutant)
        print(f"{mutant}: {status}, {duration:.2f} сек")
        summary[status] += 1
        total_time += duration

    total = sum(summary.values())
    score = (summary["killed"] / total) * 100 if total > 0 else 0

    print("\n=== ИТОГ ===")
    print(f"Mutation score: {score:.2f}%")
    print(f"Killed: {summary['killed']}")
    print(f"Survived: {summary['survived']}")
    print(f"Timeout: {summary['timeout']}")
    print(f"Compile errors: {summary['compile_error']}")
    print(f"Total time: {total_time:.2f} сек")

if __name__ == "__main__":
    main()
