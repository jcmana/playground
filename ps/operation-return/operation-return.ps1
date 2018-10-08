Operation -Summary "How does Operation block handle return" -ScriptBlock {

	Log-Info "Some code and output inside Operation"
	return
	Log-Info "Code after the return"
}

Operation -Summary "How does Operation block handle break" -ScriptBlock {

	Log-Info "Some code and output inside Operation"
	break
	Log-Info "Code after break"	
}

# Operation seems to handle both return and break same way - Operation ScripBlock is terminating and script execution continues
