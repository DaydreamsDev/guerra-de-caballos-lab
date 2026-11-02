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
EXPECTED_LESSON_COUNT = 51
MARKDOWN_LINK_PATTERN = re.compile(r"!?\[[^\]]*\]\(([^)]+)\)")
errors = []


def is_ignored(file_path):
    return any(part in IGNORED_PARTS for part in file_path.parts)


def markdown_files():
    for file_path in sorted(ROOT.rglob("*.md")):
        if not is_ignored(file_path):
            yield file_path


def check_links(document_path, document_text):
    for link_match in MARKDOWN_LINK_PATTERN.finditer(document_text):
        link_target = link_match.group(1).strip()
        if link_target.startswith(("http://", "https://", "#", "mailto:")):
            continue
        if " " in link_target:
            continue
        link_target = link_target.split("#", 1)[0]
        if not link_target:
            continue
        if not (document_path.parent / link_target).resolve().exists():
            errors.append(f"{document_path}: missing link target {link_target}")


def check_pairs(document_path):
    if document_path.name.endswith("-es.md"):
        return
    if document_path.name not in CANONICAL_NAMES:
        return
    pair_path = document_path.with_name(document_path.stem + "-es.md")
    if not pair_path.exists():
        errors.append(f"{document_path}: missing Spanish pair {pair_path.name}")


def check_fences(document_path, document_text):
    fence_count = sum(
        1
        for line in document_text.splitlines()
        if line.lstrip().startswith(("```", "~~~"))
    )
    if fence_count % 2 != 0:
        errors.append(f"{document_path}: odd number of code fences")


def check_markdown_line_length(document_path, document_text):
    for line_number, line in enumerate(document_text.splitlines(), 1):
        if len(line) > 100 and not line.startswith("|"):
            errors.append(f"{document_path}:{line_number}: Markdown line exceeds 100 columns")


def check_code_line_length(code_path):
    for line_number, line in enumerate(code_path.read_text(encoding="utf-8").splitlines(), 1):
        if len(line) > 100:
            errors.append(f"{code_path}:{line_number}: code line exceeds 100 columns")


def lesson_directories():
    directories = []
    for track in ("common", "construction", "archaeology"):
        directories.extend((ROOT / track / "lessons").glob("*"))
    return sorted(directory for directory in directories if directory.is_dir())


def lesson_catalogs():
    catalog_directory = ROOT / "docs" / "guide"
    return (
        (catalog_directory / "lesson-catalog.md", "lesson.md"),
        (catalog_directory / "lesson-catalog-es.md", "lesson-es.md"),
    )


def check_lessons():
    directories = lesson_directories()
    if len(directories) != EXPECTED_LESSON_COUNT:
        errors.append(
            f"expected {EXPECTED_LESSON_COUNT} lesson directories, found {len(directories)}"
        )
    for directory in directories:
        for name in ("meta.yml", "lesson.md", "lesson-es.md", "EVIDENCE.md", "EVIDENCE-es.md"):
            if not (directory / name).exists():
                errors.append(f"{directory}: missing {name}")


def check_metadata():
    allowed_status = {"planned", "drafted", "in review", "stable"}
    for metadata_path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
        status = metadata.get("status")
        if status not in allowed_status:
            errors.append(f"{metadata_path}: invalid status {status!r}")
        for field in ("id", "title", "concept", "prerequisites", "estimated_time", "reading_time"):
            if field not in metadata:
                errors.append(f"{metadata_path}: missing metadata field {field}")


def check_active_time():
    for metadata_path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
        total_minutes = 0
        lesson_path = metadata_path.parent / "lesson.md"
        for line in lesson_path.read_text(encoding="utf-8").splitlines():
            if re.match(r"^## Step\s+\d+", line):
                minute_ranges = re.findall(r"(\d+)(?:[–-](\d+))?\s*min", line)
                if minute_ranges:
                    low_minutes, high_minutes = minute_ranges[0]
                    total_minutes += (
                        int(low_minutes)
                        if not high_minutes
                        else (int(low_minutes) + int(high_minutes)) // 2
                    )
        expected_minutes = int(str(metadata["estimated_time"]).split()[0])
        if total_minutes != expected_minutes:
            errors.append(
                f"{metadata_path}: estimated_time {expected_minutes} "
                f"does not match step total {total_minutes}"
            )


def check_catalog_coverage():
    directories = lesson_directories()
    for catalog_path, lesson_file_name in lesson_catalogs():
        catalog_text = catalog_path.read_text(encoding="utf-8")
        linked_targets = {
            (catalog_path.parent / link_match.group(1)).resolve()
            for link_match in MARKDOWN_LINK_PATTERN.finditer(catalog_text)
        }
        for directory in directories:
            if (directory / lesson_file_name).resolve() not in linked_targets:
                errors.append(f"{catalog_path}: missing link to {directory.name}")


def main():
    for document_path in markdown_files():
        document_text = document_path.read_text(encoding="utf-8")
        check_links(document_path, document_text)
        check_pairs(document_path)
        check_fences(document_path, document_text)
        check_markdown_line_length(document_path, document_text)
    for code_path in sorted(ROOT.rglob("*")):
        if not code_path.is_file() or is_ignored(code_path):
            continue
        if code_path.suffix in CODE_SUFFIXES:
            check_code_line_length(code_path)
    check_lessons()
    check_metadata()
    check_active_time()
    check_catalog_coverage()
    if errors:
        print("\n".join(errors))
        return 1
    print(
        "QA OK: links, pairs, fences, line lengths, metadata, "
        "active times, and catalog coverage"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
