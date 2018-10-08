try
{
	Write-Host "try block"

	throw "Some exception"

}
finally
{
	Write-Host "finally block"
}