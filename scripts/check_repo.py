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
LESSON_DIRECTORY_PATTERN = re.compile(
    r"^(common|construction|archaeology)-\d{2}-[a-z0-9]+(?:-[a-z0-9]+)*$"
)
BINARY_SIGNATURES = (
    b"\x7fELF",
    b"\xfe\xed\xfa\xce",
    b"\xfe\xed\xfa\xcf",
    b"\xce\xfa\xed\xfe",
    b"\xcf\xfa\xed\xfe",
    b"\xca\xfe\xba\xbe",
)
PE_SIGNATURE = b"PE\x00\x00"
MARKDOWN_LINK_PATTERN = re.compile(r"!?\[[^\]]*\]\(([^)]+)\)")
LESSON_ID_PATTERN = re.compile(r"(?:common|construction|archaeology)-\d+")
BACKTICKED_LESSON_ID_PATTERN = re.compile(
    r"`((?:common|construction|archaeology)-\d+)`"
)
CATALOG_ROW_PATTERN = re.compile(
    r"^\| \[((?:common|construction|archaeology)-\d+)\]"
)
ALLOWED_LESSON_ROLES = {"core", "optional", "capstone"}
LESSON_ROLE_LABEL_PATTERN = re.compile(
    r"\*\*(?:Lesson role|Rol de la lección):\*\*\s*([^·\n]+)"
)
OPTIONAL_SECTION_PATTERN = re.compile(
    r"\*\*(?:Optional lessons \(safe to skip\)|Lecciones opcionales \(puedes omitirlas\)):\*\*"
)
PREREQUISITE_LABEL_PATTERN = re.compile(
    r"\*\*(?:Prerequisites?|Prerrequisitos?|Requisito previo):\*\*"
)
INTENTIONALLY_ABSENT_IDENTIFIERS = {"construction-25"}
HEADER_LINE_LIMIT = 20
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


def is_binary_file(file_path):
    with file_path.open("rb") as file_handle:
        header = file_handle.read(1024)
    if header.startswith(BINARY_SIGNATURES):
        return True
    return header.startswith(b"MZ") and PE_SIGNATURE in header


def check_committed_binaries():
    for file_path in sorted(ROOT.rglob("*")):
        if not file_path.is_file() or is_ignored(file_path):
            continue
        if is_binary_file(file_path):
            errors.append(f"{file_path}: binary file committed to the repository")


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
    lesson_ids = {}
    for directory in lesson_directories():
        required_names = (
            "meta.yml",
            "lesson.md",
            "lesson-es.md",
            "EVIDENCE.md",
            "EVIDENCE-es.md",
        )
        for name in required_names:
            if not (directory / name).exists():
                errors.append(f"{directory}: missing {name}")

        if not LESSON_DIRECTORY_PATTERN.match(directory.name):
            errors.append(f"{directory}: invalid lesson directory name")

        metadata_path = directory / "meta.yml"
        if not metadata_path.exists():
            continue
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8")) or {}
        lesson_id = metadata.get("id")
        if not lesson_id:
            continue
        if not directory.name.startswith(f"{lesson_id}-"):
            errors.append(
                f"{metadata_path}: id {lesson_id!r} does not match {directory.name!r}"
            )
        if lesson_id in lesson_ids:
            errors.append(
                f"{metadata_path}: duplicate id {lesson_id!r} "
                f"(also in {lesson_ids[lesson_id]})"
            )
        lesson_ids[lesson_id] = directory
    return lesson_ids


def check_metadata(lesson_ids):
    allowed_status = {"planned", "drafted", "in review", "stable"}
    required_fields = (
        "id",
        "title",
        "concept",
        "lesson_role",
        "prerequisites",
        "estimated_time",
        "reading_time",
    )
    for metadata_path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
        status = metadata.get("status")
        if status not in allowed_status:
            errors.append(f"{metadata_path}: invalid status {status!r}")
        lesson_role = metadata.get("lesson_role")
        if lesson_role is not None and lesson_role not in ALLOWED_LESSON_ROLES:
            errors.append(f"{metadata_path}: invalid lesson_role {lesson_role!r}")
        for field in required_fields:
            if field not in metadata:
                errors.append(f"{metadata_path}: missing metadata field {field}")
        for prerequisite in metadata.get("prerequisites") or []:
            for candidate in str(prerequisite).split(" or "):
                if candidate.strip() not in lesson_ids:
                    errors.append(
                        f"{metadata_path}: unknown prerequisite {candidate.strip()!r}"
                    )


def check_lesson_role_headers(lesson_ids):
    english_labels = {"core": "core", "optional": "optional", "capstone": "capstone"}
    spanish_labels = {
        "core": "tronco",
        "optional": "opcional",
        "capstone": "proyecto final",
    }
    documents = (
        ("lesson.md", english_labels),
        ("lesson-es.md", spanish_labels),
    )
    for directory in lesson_ids.values():
        metadata_path = directory / "meta.yml"
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8")) or {}
        lesson_role = metadata.get("lesson_role")
        if lesson_role not in ALLOWED_LESSON_ROLES:
            continue
        for document_name, labels in documents:
            document_path = directory / document_name
            if not document_path.exists():
                continue
            header_lines = document_path.read_text(encoding="utf-8").splitlines()
            header_lines = header_lines[:HEADER_LINE_LIMIT]
            declared_labels = []
            for line in header_lines:
                label_match = LESSON_ROLE_LABEL_PATTERN.search(line)
                if label_match:
                    declared_labels.append(label_match.group(1).strip().lower())
            expected_label = labels[lesson_role]
            if len(declared_labels) != 1:
                errors.append(
                    f"{document_path}: expected exactly one lesson role label, "
                    f"found {len(declared_labels)}"
                )
                continue
            if declared_labels[0] != expected_label:
                errors.append(
                    f"{document_path}: lesson role {declared_labels[0]!r} "
                    f"does not match meta.yml {expected_label!r}"
                )


def optional_lists_in_document(document_path):
    document_text = document_path.read_text(encoding="utf-8")
    optional_lists = []
    for section in OPTIONAL_SECTION_PATTERN.split(document_text)[1:]:
        section_lines = []
        for line in section.splitlines():
            if not line.strip():
                break
            section_lines.append(line)
        optional_lists.append(
            set(BACKTICKED_LESSON_ID_PATTERN.findall("\n".join(section_lines)))
        )
    return optional_lists


def expected_optional_lists(lesson_ids):
    optional_ids_by_track = {"construction": [], "archaeology": []}
    for lesson_id, directory in lesson_ids.items():
        metadata_path = directory / "meta.yml"
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8")) or {}
        if metadata.get("lesson_role") != "optional":
            continue
        track = lesson_id.split("-", 1)[0]
        if track in optional_ids_by_track:
            optional_ids_by_track[track].append(lesson_id)
    return (
        sorted(optional_ids_by_track["construction"]),
        sorted(optional_ids_by_track["archaeology"]),
    )


def check_optional_role_lists(lesson_ids):
    construction_optional_ids, archaeology_optional_ids = expected_optional_lists(lesson_ids)
    authoritative_lists = {
        "construction": set(construction_optional_ids),
        "archaeology": set(archaeology_optional_ids),
    }
    document_expectations = (
        ("START-HERE.md", ("construction", "archaeology")),
        ("START-HERE-es.md", ("construction", "archaeology")),
        ("docs/guide/lesson-catalog.md", ("construction", "archaeology")),
        ("docs/guide/lesson-catalog-es.md", ("construction", "archaeology")),
        ("docs/guide/construction-track.md", ("construction",)),
        ("docs/guide/construction-track-es.md", ("construction",)),
        ("docs/guide/archaeology-track.md", ("archaeology",)),
        ("docs/guide/archaeology-track-es.md", ("archaeology",)),
        ("construction/README.md", ("construction",)),
        ("construction/README-es.md", ("construction",)),
        ("archaeology/README.md", ("archaeology",)),
        ("archaeology/README-es.md", ("archaeology",)),
    )
    for relative_path, track_order in document_expectations:
        document_path = ROOT / relative_path
        if not document_path.exists():
            errors.append(f"{document_path}: missing optional lesson document")
            continue
        sections = optional_lists_in_document(document_path)
        if len(sections) != len(track_order):
            errors.append(
                f"{document_path}: expected {len(track_order)} optional sections, "
                f"found {len(sections)}"
            )
            continue
        for section_index, track in enumerate(track_order):
            section = sections[section_index]
            expected_ids = authoritative_lists[track]
            missing_ids = sorted(expected_ids - section)
            extra_ids = sorted(section - expected_ids)
            if not missing_ids and not extra_ids:
                continue
            other_lists = [
                authoritative_lists[other_track]
                for other_track in track_order
                if other_track != track
            ]
            if any(section == other_list for other_list in other_lists):
                errors.append(
                    f"{document_path}: optional section {section_index + 1} "
                    f"belongs to another track"
                )
                continue
            if missing_ids:
                errors.append(
                    f"{document_path}: optional section {section_index + 1} "
                    f"missing {missing_ids}"
                )
            if extra_ids:
                errors.append(
                    f"{document_path}: optional section {section_index + 1} "
                    f"has extra {extra_ids}"
                )


def expected_prerequisites(metadata):
    return {
        candidate.strip()
        for prerequisite in metadata.get("prerequisites") or []
        for candidate in str(prerequisite).split(" or ")
    }


def check_prerequisite_roles(lesson_ids):
    role_by_lesson = {}
    prerequisites_by_lesson = {}
    for lesson_id, directory in lesson_ids.items():
        metadata_path = directory / "meta.yml"
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8")) or {}
        role_by_lesson[lesson_id] = metadata.get("lesson_role")
        prerequisites_by_lesson[lesson_id] = expected_prerequisites(metadata)
    for lesson_id, lesson_role in role_by_lesson.items():
        if lesson_role not in ("core", "capstone"):
            continue
        metadata_path = lesson_ids[lesson_id] / "meta.yml"
        pending = list(prerequisites_by_lesson[lesson_id])
        visited = set()
        while pending:
            ancestor = pending.pop()
            if ancestor in visited:
                continue
            visited.add(ancestor)
            if role_by_lesson.get(ancestor) == "optional":
                errors.append(
                    f"{metadata_path}: {lesson_id} depends on optional lesson {ancestor}"
                )
            pending.extend(prerequisites_by_lesson.get(ancestor, set()))


def declared_prerequisites(document_path):
    lines = document_path.read_text(encoding="utf-8").splitlines()[:HEADER_LINE_LIMIT]
    for index, line in enumerate(lines):
        label_match = PREREQUISITE_LABEL_PATTERN.search(line)
        if not label_match:
            continue
        value = line[label_match.end():]
        cursor = index + 1
        while cursor < len(lines) and lines[cursor].startswith(">"):
            value += " " + lines[cursor].lstrip("> ").rstrip()
            cursor += 1
        value = value.split("·")[0].strip()
        if value.lower() in ("none", "ninguno", ""):
            return set()
        return set(LESSON_ID_PATTERN.findall(value))
    return None


def check_prerequisite_documents():
    for metadata_path in sorted(ROOT.glob("*/lessons/*/meta.yml")):
        metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
        expected = expected_prerequisites(metadata)
        directory = metadata_path.parent
        for name in ("lesson.md", "lesson-es.md", "README.md", "README-es.md"):
            document_path = directory / name
            if not document_path.exists():
                continue
            declared = declared_prerequisites(document_path)
            if declared is None:
                errors.append(f"{document_path}: missing prerequisite field")
            elif declared != expected:
                errors.append(
                    f"{document_path}: prerequisites {sorted(declared)} "
                    f"do not match meta.yml {sorted(expected)}"
                )


def check_catalog_prerequisites(lesson_ids):
    for catalog_path, _ in lesson_catalogs():
        for line in catalog_path.read_text(encoding="utf-8").splitlines():
            row_match = CATALOG_ROW_PATTERN.match(line)
            if not row_match:
                continue
            lesson_id = row_match.group(1)
            directory = lesson_ids.get(lesson_id)
            if directory is None:
                continue
            metadata_path = directory / "meta.yml"
            metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
            expected = expected_prerequisites(metadata)
            prerequisite_cell = line.strip("|").split("|")[3]
            declared = set(LESSON_ID_PATTERN.findall(prerequisite_cell))
            if declared != expected:
                errors.append(
                    f"{catalog_path}: {lesson_id} prerequisites {sorted(declared)} "
                    f"do not match meta.yml {sorted(expected)}"
                )


def check_catalog_roles(lesson_ids):
    for catalog_path, _ in lesson_catalogs():
        for line in catalog_path.read_text(encoding="utf-8").splitlines():
            row_match = CATALOG_ROW_PATTERN.match(line)
            if not row_match:
                continue
            lesson_id = row_match.group(1)
            directory = lesson_ids.get(lesson_id)
            if directory is None:
                continue
            metadata_path = directory / "meta.yml"
            metadata = yaml.safe_load(metadata_path.read_text(encoding="utf-8"))
            expected_role = metadata.get("lesson_role")
            declared_role = line.strip("|").split("|")[-1].strip()
            if declared_role != expected_role:
                errors.append(
                    f"{catalog_path}: {lesson_id} role {declared_role!r} "
                    f"does not match meta.yml {expected_role!r}"
                )


def check_lesson_identifiers(lesson_ids):
    for document_path in markdown_files():
        document_text = document_path.read_text(encoding="utf-8")
        for line_number, line in enumerate(document_text.splitlines(), 1):
            for identifier in BACKTICKED_LESSON_ID_PATTERN.findall(line):
                if identifier in lesson_ids:
                    continue
                if identifier in INTENTIONALLY_ABSENT_IDENTIFIERS:
                    continue
                errors.append(
                    f"{document_path}:{line_number}: "
                    f"unknown lesson identifier {identifier}"
                )


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
    lesson_ids = check_lessons()
    check_metadata(lesson_ids)
    check_lesson_role_headers(lesson_ids)
    check_optional_role_lists(lesson_ids)
    check_prerequisite_roles(lesson_ids)
    check_active_time()
    check_catalog_coverage()
    check_prerequisite_documents()
    check_catalog_prerequisites(lesson_ids)
    check_catalog_roles(lesson_ids)
    check_lesson_identifiers(lesson_ids)
    check_committed_binaries()
    if errors:
        print("\n".join(errors))
        return 1
    print(
        "QA OK: links, pairs, fences, line lengths, metadata, active times, "
        "catalog coverage, catalog roles, prerequisites, lesson roles, "
        "optional lists, prerequisite roles, lesson identifiers, and committed binaries"
    )
    return 0


if __name__ == "__main__":
    sys.exit(main())
