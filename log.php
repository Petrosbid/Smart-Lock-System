<?php
$logfile = 'log.txt';

if ($_SERVER['REQUEST_METHOD'] == 'POST') {
    if (isset($_POST['log'])) {
        $log = $_POST['log'];
        $log_entry = date('Y-m-d H:i:s') . " - " . $log . PHP_EOL;
        file_put_contents($logfile, $log_entry, FILE_APPEND);
        echo json_encode(['success' => true, 'message' => 'Log saved']);
    } else {
        echo json_encode(['success' => false, 'message' => 'No log data received']);
    }
} else if ($_SERVER['REQUEST_METHOD'] == 'GET') {
    if (file_exists($logfile)) {
        $logs = file($logfile, FILE_IGNORE_NEW_LINES | FILE_SKIP_EMPTY_LINES);
        echo json_encode($logs);
    } else {
        echo json_encode([]);
    }
} else {
    echo json_encode(['success' => false, 'message' => 'Invalid request method']);
}
?>
