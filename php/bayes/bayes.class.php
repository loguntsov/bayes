<?php
/**
 * Классификатор байеса работающий
 */
class bayes {
	protected $config_file;
	protected $pipe_in;
	protected $pipe_out;
	protected $process;
	protected $params;

	public function __construct($file, $params = '') {
		$this->config_file = realpath($file);
		$this->params = $params;
		$this->open();
	}

	public function __destroy() {
		$this->close();
	}

	public static function bayesExecution($mode, $file, $params) {
		return ROOT_DIR."/core/classes/bayes/bayes  $mode '{$file}' ".$params;
	}

	protected function open() {
		$description = array(
		   0 => array("pipe", "r"),  // stdin is a pipe that the child will read from
		   1 => array("pipe", "w"),  // stdout is a pipe that the child will write to
		   2 => array("file", "php://stderr", "a") // stderr is a file to write to
		);

		$pipes = array();
		$run = static::bayesExecution('C',$this->config_file,$this->params);
		$this->process = proc_open($run, $description, $pipes);
		//$this->process = proc_open('cat /tmp/1.txt', $description, $pipes);

		$this->pipe_in = $pipes[0];
		$this->pipe_out = $pipes[1];

		return is_resource($this->process);
	}

	protected function close() {
		if (is_resource($this->process)) {
			fclose($this->pipe_in);
			fclose($this->pipe_out);
			proc_close($this->process);
		}
		$this->process = NULL;
	}

	public function classify($text) {
		fwrite($this->pipe_in, $text."\0");
		$rows = array();
		do {
			$row = trim(fgets($this->pipe_out));
			if ($row == '') break;
			list($type, $estimation, $p) = explode(' ', $row);
			$rows[$type] = array((float) $estimation, (float) $p);
		} while($row != '');
		return $rows;
	}

	public function getType($text, $threshold = 0.7) {
		$estimation = $this->classify($text);
		reset($estimation);
		$a = current($estimation);
		$max = $a[1];
		$maxEst = key($estimation);
		foreach($estimation as $type=>$est) {
			if ($max < $est[1]) {
				$max = $est[1];
				$maxEst = $type;
			}
		}
		if ($max < $threshold) return false;
		return $maxEst;
	}
}
