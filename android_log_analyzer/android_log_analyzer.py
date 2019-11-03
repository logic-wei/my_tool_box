# -*-coding:utf-8-*-
import xlwt
import os
import re
import abc
from string import Template


class LogFile:
    class ResultContainer:
        @abc.abstractmethod
        def add(self, title, backtrace, asaninfo):
            pass

    def __init__(self, name, path, result_container=ResultContainer()):
        self.path = path
        self.name = name
        self.result_container = result_container

    def set_result_container(self, result_container):
        self.result_container = result_container

    @staticmethod
    def get_log_file_list(path, is_bsp_test_report=False, filter_pattern=None):
        log_files = []

        logs_root_path = path
        if is_bsp_test_report:
            logs_root_path = os.path.join(path, "BSPTESTREPORT/BSPLog")

        for file in os.listdir(logs_root_path):
            file_path = os.path.join(logs_root_path, file)

            if os.path.isfile(file_path):
                if filter_pattern:
                    if not re.match(filter_pattern, file):
                        continue
                log_files.append(LogFile(file, file_path))

        return log_files

    def parse_backtrace(self):
        result = None

        str_header = "*** *** *** *** *** *** *** *** *** *** *** *** *** *** *** ***"
        str_f_debug = "F DEBUG"
        pattern_backtrace = re.compile(r".*backtrace:.*")
        pattern_pc = re.compile(r".*\s+#\d+\s+pc\s+[0-9a-fA-F]+\s+.*")

        with open(self.path, mode="rt", encoding="ISO-8859-1") as log:
            in_backtrace = False
            in_pc = False
            # Used to filtrate log not related to backtrace
            is_f_debug_mode = False
            for line in log:
                if not in_backtrace:
                    if str_header in line:
                        in_backtrace = True
                        result = ""
                        if str_f_debug in line:
                            is_f_debug_mode = True
                else:
                    if len(result) >= 12000:
                        break
                    if is_f_debug_mode:
                        if str_f_debug not in line:
                            continue
                    if not in_pc:
                        result += line
                        if pattern_backtrace.match(line):
                            in_pc = True
                    else:
                        if not pattern_pc.match(line):
                            break
                        result += line
        return result

    def parse_asaninfo(self):
        result = None
        str_header = "AddressSanitizer"
        pattern_process_name = re.compile(r".*android\.hardware\.camera\.provider@\d+\.\d+-service.*")
        pattern_ending = re.compile(r".*ABORTING.*")

        with open(self.path, mode="rt", encoding="ISO-8859-1") as log:
            in_asaninfo = False
            for line in log:
                if not in_asaninfo:
                    if str_header in line:
                        in_asaninfo = True
                        result = line
                else:
                    if len(result) >= 12000:
                        break
                    if pattern_process_name.match(line):
                        result += line
                        if pattern_ending.match(line):
                            break
        return result

    def parse(self):
        backtrace = self.parse_backtrace()
        asaninfo = self.parse_asaninfo()
        self.result_container.add(self.name, backtrace, asaninfo)


class ExcelContainer(LogFile.ResultContainer):
    # Configs
    CONFIG_COL_NUM = 3

    def __init__(self, filename="log-analysis-result.xls"):
        self.filename = filename
        self.workbook = xlwt.Workbook(encoding="utf-8")
        self.worksheet = self.workbook.add_sheet("Analysis result")
        self.position_row = 0

        self.worksheet.col(0).width = 10000
        self.worksheet.col(1).width = 30000
        self.worksheet.col(2).width = 30000

        # Title init
        pattern = xlwt.Pattern()
        pattern.pattern = xlwt.Pattern.SOLID_PATTERN
        # May be: 8 through 63. 0 = Black, 1 = White, 2 = Red,
        # 3 = Green, 4 = Blue, 5 = Yellow, 6 = Magenta, 7 = Cyan,
        # 16 = Maroon, 17 = Dark Green, 18 = Dark Blue, 19 = Dark Yellow ,
        # almost brown), 20 = Dark Magenta, 21 = Teal, 22 = Light Gray,
        # 23 = Dark Gray, the list goes on...
        pattern.pattern_fore_colour = 52
        alignment = xlwt.Alignment()
        alignment.horz = xlwt.Alignment.HORZ_CENTER
        alignment.vert = xlwt.Alignment.VERT_CENTER
        style = xlwt.XFStyle()
        style.pattern = pattern
        style.alignment = alignment

        self.worksheet.write_merge(self.position_row, 0, self.position_row, self.CONFIG_COL_NUM-1,
                                   "Log analysis result", style=style)
        self.position_row += 1

        # Sub title init
        pattern = xlwt.Pattern()
        pattern.pattern = xlwt.Pattern.SOLID_PATTERN
        pattern.pattern_fore_colour = 7
        style = xlwt.XFStyle()
        style.pattern = pattern
        self.worksheet.write(self.position_row, 0, "Title", style=style)
        self.worksheet.write(self.position_row, 1, "Backtrace", style=style)
        self.worksheet.write(self.position_row, 2, "ASan Info", style=style)
        self.position_row += 1

    def save(self):
        self.workbook.save(self.filename)

    def add(self, title, backtrace=None, asaninfo=None):
        normal_pattern = xlwt.Pattern()
        normal_pattern.pattern = xlwt.Pattern.SOLID_PATTERN
        normal_pattern.pattern_fore_colour = 1
        normal_style = xlwt.XFStyle()
        normal_style.pattern = normal_pattern

        error_pattern = xlwt.Pattern()
        error_pattern.pattern = xlwt.Pattern.SOLID_PATTERN
        error_pattern.pattern_fore_colour = 27
        error_style = xlwt.XFStyle()
        error_style.pattern = error_pattern

        title_style = normal_style
        backtrace_style = normal_style
        asaninfo_style = normal_style
        if backtrace is not None or asaninfo is not None:
            title_style = error_style
        if backtrace is not None:
            backtrace_style = error_style
        if asaninfo is not None:
            asaninfo_style = error_style

        self.worksheet.write(self.position_row, 0, label=title, style=title_style)
        self.worksheet.write(self.position_row, 1, label=backtrace, style=backtrace_style)
        self.worksheet.write(self.position_row, 2, label=asaninfo, style=asaninfo_style)
        self.position_row += 1


class HtmlContainer(LogFile.ResultContainer):
    def __init__(self, filename="log-analysis-result.html"):
        self.filename = filename
        self.file = None

    def begin_add(self):
        self.file = open(self.filename, "wt", encoding="utf-8")
        html_begin = """
<html>
<style type="text/css">
    table {
        background: rgb(247, 247, 247);
        border-collapse: collapse;
        margin: 0 auto;
        text-align: center;
    }

    table th {
        border: 0mm solid rgb(255, 255, 255);
        color: #666;
        height: 1cm;
        margin: 0px;
    }

    table td {
        border: 2mm solid #ffffff;
        color: #666;
        height: 1cm;
        margin: 0px;
        max-width: 5cm;
        word-wrap: break-word;
    }

    textarea {
        border: 0cm;
        color: #919191;
        background: rgb(247, 247, 247);
        outline: none;
        width: 13cm;
        height: 6cm;
        resize: none;
    }
</style>

<body>
    <table>
        <tr>
            <th colspan="3">
                Log Analysis Result
            </th>
        </tr>
        <tr style="background: rgb(212, 212, 212);">
            <th style="width: 5cm;min-width: 5cm;">
                File
            </th>
            <th style="width: 14cm;min-width: 14cm;">
                Backtrace
            </th>
            <th style="width: 14cm;min-width: 14cm;">
                ASan Info
            </th>
        </tr>
"""
        self.file.write(html_begin)

    def end_add(self):
        html_end = """
    </table>

</body>

</html>
"""
        self.file.write(html_end)
        self.file.close()

    def add(self, title, backtrace, asaninfo):
        if not backtrace:
            backtrace = ""
        if not asaninfo:
            asaninfo = ""
        self.file.write(self.build_row_html(title, backtrace, asaninfo))

    def build_row_html(self, title, backtrace, asaninfo):
        template_str_row = """
        <tr>
            <td style="color: $color;">
                $title
            </td>
            <td>
                <textarea readonly>
$backtrace
            </textarea>
            </td>
            <td>
                <textarea readonly>
$asaninfo
            </textarea>
            </td>
        </tr>
"""
        template_row = Template(template_str_row)
        color = "#666"
        if backtrace or asaninfo:
            color = "#FF6347"

        return template_row.substitute(color=color, title=title, backtrace=backtrace, asaninfo=asaninfo)


def generate_html(log_files):
    result_file = HtmlContainer()
    result_file.begin_add()
    for file in log_files:
        print("Parsing: " + file.name)
        file.set_result_container(result_file)
        file.parse()
    result_file.end_add()
    print("Html has been generated: " + result_file.filename)


def generate_xls(log_files):
    result_file = ExcelContainer()
    for file in log_files:
        print("Parsing: " + file.name)
        file.set_result_container(result_file)
        file.parse()
    result_file.save()
    print("Xls has been generated: " + result_file.filename)


log_files = LogFile.get_log_file_list("/Users/logic/Downloads/Ultra_U21030ASAN/BSPTESTREPORT/BSPLog")
generate_html(log_files)
