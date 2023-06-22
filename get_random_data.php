<?php
// Menghasilkan nilai acak untuk kedua grafik
$value1 = rand(0, 100);
$value2 = rand(0, 100);

// Mengembalikan data dalam format JSON
$data = array(
    'value1' => $value1,
    'value2' => $value2
);

header('Content-Type: application/json');
echo json_encode($data);
?>
