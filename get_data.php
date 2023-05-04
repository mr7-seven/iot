<?php
header('Content-Type: application/json');

// baca data dari file dataku.json
$data = json_decode(file_get_contents('dataku.json'), true);

// baca data dari request POST dalam format JSON
$json = file_get_contents('php://input');
$postData = json_decode($json, true);

// periksa apakah ada data POST yang diterima
if (!empty($postData)) {
  // ambil nilai suhu, kelembapan, dan lux dari data POST
  $suhu = $postData['suhu'];
  $kelembapan = $postData['kelembapan'];
  $lux = $postData['lux'];

  // update nilai suhu, kelembapan, dan lux di variabel $data
  $data['suhu'] = $suhu;
  $data['kelembapan'] = $kelembapan;
  $data['lux'] = $lux;

  // simpan kembali data ke dalam file dataku.json
  $result = file_put_contents('dataku.json', json_encode($data, JSON_PRETTY_PRINT));

  // cek apakah data berhasil diupdate
  if ($result !== false) {
    $response = array('status' => 'success', 'pesan' => 'Data berhasil diperbarui.');
  } else {
    $response = array('status' => 'error', 'pesan' => 'Terjadi kesalahan saat memperbarui data.');
  }
} else {
  $response = array('status' => 'error', 'pesan' => 'Data tidak ditemukan atau permintaan tidak valid.');
}

// kirim response dalam format JSON
echo json_encode($response);
