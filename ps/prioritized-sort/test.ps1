. .\psort.ps1


$local:list = @(
	"aaa",
	"aab",
	"aax",
	"baa",
	"bbb"
	"xbb",
	"aza"
)

$list | PrioritySort-Object {$_ -cmatch "x.."} | Out-File "out.txt" -Encoding "UTF8"