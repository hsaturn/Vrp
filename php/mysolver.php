<?php

function getWord(&$s, $sep=' ')
{
	$pos=strpos($s,$sep);
	if ($pos===false)
	{
		$ret=$s;
		$s='';
	}
	else
	{
		$ret=trim(substr($s,0,$pos));
		$s=trim(substr($s,$pos+1));
	}
	return $ret;
}

class server
{
	public function server($port=3333)
	{
		echo "OPENING PORT $port\n";
		$this->port=$port;
		$this->sock=fsockopen("127.0.0.1",$port);
	}

	public function __destruct()
	{
		echo "CLOSING CONNEXION\n".$this->sock.PHP_EOL;
		fclose($this->sock);
		// Sauf que socket_close ne fonctionne pas sur mon putain de pc
	}

	public function send($cmd)
	{
		echo "SENDING $cmd\n";
		fputs($this->sock, $cmd."\n");
		usleep(100000);
	}

	public function sendWait($cmd)
	{
		$this->send($cmd);
		return $this->wait(getWord($cmd));
	}

	public function colors()
	{
		$colors = $this->sendWait("colors");
		$a = explode(' ', $colors);
		$colors=$a[3];
		if (strlen($colors)==54)
			return $colors;

		print "ERROR REDING COLORS\n";
		return "";
	}

	public function get()
	{
		/*
		$a[]=$this->sock;
		$b=NULL;
		if (stream_select($a, $b, $b, 0)==0)
		{
			sleep(1);
			return "";
		}	*/
		$incoming = trim(fgets($this->sock));
		$this->parse($incoming);
		return $incoming;
	}

	public function parse($incoming)
	{

	}

	/**
	 * Attent un #OK func et retourne le résultat
	 **/
	public function wait($fonc)
	{
		while(true)
		{
			$ret=$this->get();
			$ret=trim($ret);
			if (strpos($ret, "#OK $fonc")===0)
			{
				print "OK $fonc\n";
				return $ret;
			}
			else if (strlen($ret))
				print "Ignoring ($ret)\n";
		}
	}

	public function find($c1, $c2="", $c3="")
	{
		$this->send("find $c1 $c2 $c3");
		$rec=$this->wait("find");
		if (strpos($rec,"=")!==0)
		{
			$rec=substr($rec,strpos($rec,'=')+1);
		}
		else
		{
			$this->error("BAD FIND RETURN ("+$rec+")");
		}
		return $rec;
	}

	public function error($err)
	{
		print "***\n*** ERROR $err\n***\n";
	}

	private $port=0;
	private $sock;
}

if (isset($argv[1]))
	$port=$argv[1];
else
{
	$port = file_get_contents("port.sav");
	if ($port<3000) $port=3333;
	$port++;
}
file_put_contents("port.sav",$port);

system("./cube_server > /dev/null $port &");
sleep(5);

$server = new server($port);

echo "FIND=[".($server->find("w b"))."]\n";
$faces=array('top','bottom','left','right','front','back');

function demo()
{
	global $server;
	$server->send("anim y 0.3");
	$server->send("anim x 0.05");
	while(1)
	{
		$a=explode(' ',"cube cube3 checker zigzag holes 6t stripes twisted");
		$pattern=$a[rand(0,count($a)-1)];
		$server->send("learn");
		$server->send("pattern $pattern");
		$server->send("learn end");
		sleep(20);
		$ret="x";
		while($ret!="")
		{
			$ret = $server->get();
			echo "GOT : [".$ret."]".PHP_EOL;
		} 
		$server->send("reverse learn");
	}
}

function bestTop()
{
	global $server;
	global $faces;

	$best='';
	$best_cnt=0;

	foreach($faces as $face)
	{
		$cnt=array();
		$desc=$server->sendWait("desc $face");
		print "DESC($face)=$desc\n";
		$c=explode(' ',$desc);
		foreach($c as $color)
		{
			if (!isset($cnt[$color])) $cnt[$color]=0;
			if ($color==$c[4])
				$cnt[$color]++;
		}
		$max=max($cnt);
		if ($max>$best_cnt)
			$best=$c[4];
	}
	print "BEST=$best\n";
	$server->send("top $best");
}

function solver()
{
	bestTop();
}

function match($algo, $colors)
{
	$algo=str_replace('-','',$algo);
	$mapping=array();	// Mapping de couleurs
	if (strlen($algo)!=54) print "ERROR ALGO SIZE ($algo)\n";
	else if (strlen($colors)!=54) print "ERROR COLOR SIZE ($algo)\n";
	else	
	{
		for($i=0; $i<54; $i++)
		{
			if ($algo[$i]=='.')
				$colors[$i]='.';
			else
			{
				if (!isset($mapping[$colors[$i]]))
				{
					if (in_array($algo[$i], $mapping))
					{
						break;
					}
					else
						$mapping[$colors[$i]]=$algo[$i];
				}
				$colors[$i]=$mapping[$colors[$i]];
			}
		}
		return $algo==$colors;
	}
	return false;
}

function runAlgo($c)
{
	print "------------------------\n";
	print "COURONNE $c\n";
	global $server;
	global $rotates;
	$server->sendWait("is_valid");
	$a = readAlgo($c);
	$loop=1;
	$colors="";
	while($loop)
	{
		$loop=0;
		for($rotate=0; $rotate<4; $rotate++)
		{
			foreach($a as $algo_row)
			{
				//print "  CHECKING ALGO [$algo_row]\n";
				$pos=strpos($algo_row,'#');
				if ($pos!==false)
					$algo_row=trim(substr($algo_row,0,$pos-1));
				$algo=getWord($algo_row);
				$moves=getWord($algo_row);

				if ($colors=="")
					$colors=$server->colors();
				
				if (match($algo, $colors))
				{
					$colors="";
					$loop = 1;
					global $amoves;
					$rotates=$rotates%4;
					if ($rotates>=1 && $rotates<=2)
						$amoves[count($amoves)+1]="rotate y ".(90*$rotates);
					else if ($rotates==3)
						$amoves[count($amoves)+1]="rotate y -90";
					$rotates=0;

					$amoves[count($amoves)+1]=$moves;
					print "  MATCH $algo_row\n";
					$server->send("move $moves");
				}
				if ($loop) break;
			}
			if ($loop) break;
			$server->send("rotate y 90");
			$colors="";
			$rotates++;
		}
	}
	print "END ALGO $c\n";
}

function readAlgo($c)
{
	print "READING ALGOS $c\n";
	global $server;
	$a=array();
	$f=fopen("solver.data","r");
	while(!feof($f))
	{
		$line=fgets($f);
		if (getWord($line)==$c)
			$a[]=$line;
	}
	print "READ ALGOS $c, count=".count($a).PHP_EOL;
	return $a;
}

function croix()
{
	runAlgo("0");
}

function couronne1()
{
	runAlgo("1");
}

function couronne2()
{
	runAlgo("2");
}

function couronne3()
{
	global $server;
	runAlgo("3");
	runAlgo("4");
}

function solve()
{
	global $server;
	global $amoves;
	global $rotates;
	$rotates=0;
	$amoves=array();
	croix();
	couronne1();
	couronne2();
	couronne3();

	print "----------- SOLUTION ----------\n";
	foreach($amoves as $index => $move)
		print "$index   $move\n";
	
	print $server->colors()."\n";
}

function test($init)
{
	global $server;
	static $f;
	static $irow;
	static $algo="";

	if ($init===true)
	{
		$f=fopen("solver.data","r");
		$irow=0;
		$algo="";
	}

	if (strlen($algo))
	{
		$server->send("move $algo");
		$server->send("echo Row #$irow move done");
		$algo="";
		return;
	}
	$done=false;
	while($f && !$done)
	{
		if (feof($f))
		{
			$server->send("echo END OF TEST");
			$done=true;
		}
		
		$line=fgets($f);
		$irow++;
		if (($init==0 || $irow=$init) && strlen(trim($line)))
		{
			if ($line[0]!=' ')
			{
				$server->send("echo Row #$irow : [$line]");
				$nr=getWord($line);
				$colors=getWord($line);
				$algo=getWord($line);
				$server->send("colors $colors");
				$done=true;
			}
		}
	}

}

while(true)
{
	$event = $server->get();
	echo "EVENT [ ".$event." ] ".PHP_EOL;

	if (getWord($event)=="#EVENT")
	{
		if (getWord($event)=="mousedown")
		{
			$btn=getWord($event);
			if ($btn=="demo")
				demo();
			else if ($btn=="solve")
				solve();
			else if ($btn=="tst_start")
				test(true);
			else if ($btn=="next")
				test(intval($event));
		}
	}
}

