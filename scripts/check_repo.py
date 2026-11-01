from pathlib import Path
import re
import sys
import yaml

ROOT = Path(__file__).resolve().parents[1]
IGNORED_PARTS = {".git", ".atl", ".codegraph", "build", "__pycache__"}
CANONICAL_NAMES = {
    "README.md",
    "lesson.md",
    "EVIDENCE.md",
    "REPORT-TEMPLATE.md",
    "START-HERE.md",
    "CATALOG.md",
    "AGENTS.md",
}
CODE_SUFFIXES = {".cpp", ".h", ".py", ".sh"}
errors = []


def is_ignored(path):
    return any(part in IGNORED_PARTS for part in path.parts)


def markdown_files():
    for path in sorted(ROOT.rglob("*.md")):
        if not is_ignored(path):
            yield path


def check_links(path, text):
    for match in re.finditer(r"!?\[[^\]]*\]\(([^)]+)\)", text):
        target = match.group(1).strip()
        if target.startswith(("http://", "https://", "#", "mailto:")):
            continue
        if " " in target:
            continue
        target = target.split("#", 1)[0]
        if not target:
            continue
        if not (path.parent / target).resolve().exists():
            errors.append(f"{path}: missing link target {target}")


def check_pairs(path):
    if path.name.endswith("-es.md"):
        return
    if path.name not in CANONICAL_NAMES:
        return
    pair = path.with_name(path.stem + "-es.md")
    if not pair.exists():
        errors.append(f"{path}: missing Spanish pair {pair.name}")


def check_fences(path, text):
    fence_count = sum(
        1 for line in text.splitlines() if line.lstrip().startswith(("```", "~~~"))
    )
    if fence_count % 2 != 0:
        errors.append(f"{path}: odd number of code fences")


def check_markdown_line_length(path, text):
    for number, line in enumerate(text.splitlines(), 1):
        if len(line) > 100 and not line.startswith("|"):
            errors.append(f"{path}:{number}: Markdown line exceeds 100 columns")


def check_code_line_length(path):
    for number, line in enumerate(path.read_text(encoding="utf-8").splitlines(), 1):
        if len(line) > 100:
            errors.append(f"{path}:{number}: code line exceeds 100 columns")


def check_lessons():
    lesson_directories = []
    for track in ("common", "construction", "archaeology"):
        lesson_directories.extend((ROOT / track / "lessons").glob("*"))
    lesson_directories = sorted(path for path in lesson_directories if path.is_dir())
    if len(lesson_directories) != 51:
        errors.append(f"expected 51 lesson directories, found {len(lesson_directories)}")
    for directory in lesson_directories:
        for name in ("meta.yml", "lesson.md", "lesson-es.md", "EVIDENCE.md", "EVIDENCE-es.md"):
            if not (directory / name).exists():
                errors.append(f"{directory}: missing {name}")


def check_metadata():
    allowed_status = {"planned", "drafted", "in review", "stable"}
    for path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        data = yaml.safe_load(path.read_text(encoding="utf-8"))
        status = data.get("status")
        if status not in allowed_status:
            errors.append(f"{path}: invalid status {status!r}")
        for field in ("id", "title", "concept", "prerequisites", "estimated_time", "reading_time"):
            if field not in data:
                errors.append(f"{path}: missing metadata field {field}")


def check_active_time():
    for meta_path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        data = yaml.safe_load(meta_path.read_text(encoding="utf-8"))
        total = 0
        lesson_path = meta_path.parent / "lesson.md"
        for line in lesson_path.read_text(encoding="utf-8").splitlines():
            if re.match(r"^## Step\s+\d+", line):
                values = re.findall(r"(\d+)(?:[–-](\d+))?\s*min", line)
                if values:
                    low, high = values[0]
                    total += int(low) if not high else (int(low) + int(high)) // 2
        expected = int(str(data["estimated_time"]).split()[0])
        if total != expected:
            errors.append(
                f"{meta_path}: estimated_time {expected} does not match step total {total}"
            )


def main():
    for path in markdown_files():
        text = path.read_text(encoding="utf-8")
        check_links(path, text)
        check_pairs(path)
        check_fences(path, text)
        check_markdown_line_length(path, text)
    for path in sorted(ROOT.rglob("*")):
        if not path.is_file() or is_ignored(path):
            continue
        if path.suffix in CODE_SUFFIXES:
            check_code_line_length(path)
    check_lessons()
    check_metadata()
    check_active_time()
    if errors:
        print("\n".join(errors))
        return 1
    print("QA OK: links, pairs, fences, line lengths, metadata, and active times")
    return 0


if __name__ == "__main__":
    sys.exit(main())
