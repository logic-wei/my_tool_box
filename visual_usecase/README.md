# visual_usecase
A powerful tool for parsing qcom camera usecases xml file.

## Usage
```bash
usage: visual_usecase.py [-h] [-v] [-u USECASE] file path

positional arguments:
  file                  Specify a xml file of usecases to process.
  path                  Specify a path to output.

optional arguments:
  -h, --help            show this help message and exit
  -v, --version         show program's version number and exit
  -u USECASE, --usecase USECASE
                        Specify a usecase name to process.Default all the
                        usecases.

```
### Example

If there was a file test_usecases.xml,run the command below:

```python
python3 visual_usecase usecases.xml ./out
```

Then you can get some outputs below in out:
```bash
$ cd out
$ tree
.
└── UsecasePreview
    ├── Preview
    └── Preview.pdf
```

Open the `Preview.pdf`:
![](./screenshots/Preview.jpg)