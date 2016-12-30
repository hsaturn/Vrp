<?php

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
		return trim(fgets($this->sock));
	}

	/**
	 * Attent un #OK func (ou #KO fonc) et retourne le résultat
	 **/
	public function wait($fonc)
	{
		while(true)
		{
			$ret=$this->get();
			$ret=trim($ret);
			if (strpos($ret, "#OK $func")===0)
			{
				return $ret;
			}
			else if (strlen($ret))
				print "Ignoring ($ret)\n";
		}
	}

	public function find($c1, $c2="", $c2="")
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
	$port=3500;

system("./cube_server > /dev/null $port &");
sleep(1);

$server = new server($port);

echo "FIND=[".($server->find("w b"))."]\n";

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
		$server->send("pattern $pattern noreset");
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

demo();
