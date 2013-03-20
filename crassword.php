<?php

/**
 * A PHP wrapper for crassword's HTML mode.
 * Copyright (c) 2013 by Alec Smecher
 * Released under the GPL v2 licence. See COPYING for details.
 */

$solve = isset($_GET['solve']);
$seed = isset($_GET['seed'])?((int) $_GET['seed']):rand();

passthru('./crassword --html' . ($solve?' --solve':'') . ($seed?" --seed $seed":''));

?>

<br/>

<?php if (!$solve) {?>
	<a href="crassword.php?solve&seed=<?php echo $seed; ?>">Solve</a><br/>
	<a href="crassword.php?seed=<?php echo $seed; ?>">Permalink</a><br/>
<?php } else { ?>
	<a href="crassword.php?seed=<?php echo $seed; ?>">Clear Solution</a><br/>
	<a href="crassword.php?solve&seed=<?php echo $seed; ?>">Permalink</a><br/>
<?php } ?>
<a href="crassword.php">Generate Another</a>
