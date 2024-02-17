import os
import fileinput

def ParseFile(p) -> int:
  result = 0
  for line in fileinput.input(p, encoding="utf-8"):
    if len(line) > 1:
      result += 1
  return result
    
def CountFileLines(p) -> int:
  result = 0
  with os.scandir(path=p) as it:
    for entry in it:
      if os.path.isdir(entry.path) and not entry.name.__contains__("build"):
        result += CountFileLines(entry.path)
      else: 
        if entry.name.__contains__('.cc') or entry.name.__contains__('.h'):
          result += ParseFile(entry.path)
  return result

count_lines = CountFileLines('.')

print(f"Project contains {count_lines} lines of code." )