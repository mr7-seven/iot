<?php
// Konfigurasi database
require_once('db_config.php');

try {
    $query = "SELECT * FROM tb_data";
    $stmt = $conn->query($query);

    $results = $stmt->fetch(PDO::FETCH_ASSOC);
    $json = json_encode($results);

    header('Content-Type: application/json');
    echo $json;
} catch (PDOException $e) {
    $error = array('error' => $e->getMessage());
    $json = json_encode($error);

    header('Content-Type: application/json');
    echo $json;
}
