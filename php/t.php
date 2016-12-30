<?php
$fp = fsockopen("10.4.1.188", 3333, $errno, $errstr, 30);
if (!$fp) {
	echo "$errstr ($errno)<br />\n";
} else {
	$rot=0;
		while(true)
		{
			fwrite($fp, "rotate top $rot\n");
			$rot++;
			usleep(100000);
		}
}   
fclose($fp);                                                                                                                                                                                 

