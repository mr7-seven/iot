<?php
include 'db_config.php';

function rupiah($angka){
	
	$hasil_rupiah = "Rp " . number_format($angka,2,',','.');
	return $hasil_rupiah;
}

$data = $conn->query("SELECT * FROM tb_data")->fetchAll();
// and somewhere later:
foreach ($data as $row) {
    $nama =  $row['nama'];
    $nim  = $row['nim'];
    $uid =  $row['uid'];
    $waktu =  $row['waktu'];
    $saldo =  rupiah($row['saldo']);
}
?>

<!doctype html>
<html lang="en">
  <head>
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>Latihan Database</title>
    <link href="assets/css/bootstrap.min.css" rel="stylesheet">
    <link href="assets/css/datatables.min.css" rel="stylesheet">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/sweetalert2@11.7.12/dist/sweetalert2.min.css">
  </head>
  <body>
    
  <div class="container">
  <!-- Content here -->
  <div class="row mb-3">
    <div class="col">
    <nav class="navbar bg-body-tertiary">
      <div class="container-fluid">
        <a class="navbar-brand" href="#">
          <img src="assets/images/poliban.png" alt="Logo" width="45" class="d-inline-block align-text-top">
          PEMBAYARAN TOL BANJARMASIN
        </a>
      </div>
    </nav>
    </div>
  </div>

  <div class="row">
    <div class="col-4">
    <div class="card mb-3" style="max-width: 540px;">
  <div class="row g-0">
    <div class="col-md-4">
      <img src="assets/images/poliban.png" class="img-fluid rounded-start" alt="...">
    </div>
    <div class="col-md-8">
      <div class="card-body">
        <h5 class="card-title"><?= $nama; ?></h5>
        <p class="card-text"><?= $nim; ?></p>
        <p class="card-text"><small class="text-body-secondary">Last updated <?= $waktu; ?></small></p>
      </div>
    </div>
  </div>
</div>
    </div>

    <div class="col-8">
    <table id="dataku" class="display table" style="width:100%">
  <thead>
    <tr>
      <th scope="col">#</th>
      <th scope="col">Nama</th>
      <th scope="col">NIM</th>
      <th scope="col">UID</th>
      <th scope="col">Saldo</th>
      <th scope="col">Waktu</th>
    </tr>
  </thead>
  <tbody>
  </tbody>
</table>
    </div>
</div>

<div class="row mt-3">
  <div class="col">
  <div class="card text-center">
  <div class="card-header">
    SALDO ANDA
  </div>
  <div class="card-body">
    <h2 class="card-title">SISA SALDO ANDA</h2>
    <a href="#" class="btn btn-primary fs-1"><div id="saldo-container"></div> </a>
  </div>
  <div class="card-footer text-body-secondary">
  <div id="waktu-container"></div>
  </div>
</div>
  </div>
</div>
</div>


    <script src="assets/js/bootstrap.bundle.min.js"></script>
    <script src="assets/js/jquery-3.6.4.min.js"></script>
    <script src="assets/js/datatables.min.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/sweetalert2@11.7.12/dist/sweetalert2.all.min.js"></script>

    <script>
      $(document).ready(function() {
        setInterval(updateSaldo, 2000);
      });

      /* Fungsi formatRupiah */
      const rupiah = (number)=>{
        return new Intl.NumberFormat("id-ID", {
          style: "currency",
          currency: "IDR"
        }).format(number);
      }
       var flag = false;
      function updateSaldo() {
        $.get('get_saldo.php')
          .done(function(response) {
            
            if(response.saldo <50000 && flag != true){
              Swal.fire({
              icon: 'error',
              title: 'Oops...',
              text: 'Saldo Anda Tidak Cukup',
              footer: '<a href="">Silahkan Isi Saldo Anda Ke Indomaret?</a>'
            })
            flag = true;
            $('#saldo-container').text(rupiah(0));
            }else if(response.saldo >=50000){
              $('#saldo-container').text(rupiah(response.saldo));
              $('#waktu-container').text(response.waktu);
              flag = false;
            }
            
          })
          .fail(function(xhr, status, error) {
            console.log(error);
          });
      }
    </script>

    <script>
    $('#dataku').DataTable( {
    ajax: {
        url: 'get_saldo.php',
        dataSrc: ''
    },
    columns: [  { data: 'nama' },
                { data: 'nim' },
                { data: 'uid' },
                { data: 'saldo' },
                { data: 'waktu' },
  { 
    data: null,
    render: function(data, type, row) {
      return '<button class="btn-update" data-id="' + row.id + '">Update</button>';
    }
  },]
} );
    </script>

<script>
        $(document).ready(function() {
            // Tangani klik tombol "Update"
            $('.btn-update').on('click', function() {
                var row = $(this).closest('tr'); // Ambil baris terdekat dari tombol yang diklik
                var nama = row.find('td:eq(0)').text(); // Ambil teks dari kolom Nama (indeks 0)
                var nim = row.find('td:eq(1)').text(); // Ambil teks dari kolom NIM (indeks 1)
                var kelas = row.find('td:eq(2)').text(); // Ambil teks dari kolom Kelas (indeks 2)

                // Set nilai input form dengan data dari baris tabel
                $('#nama').val(nama);
                $('#nim').val(nim);
                $('#kelas').val(kelas);

                // Tampilkan modal
                $('#updateModal').modal('show');
            });

            // Tangani klik tombol "Simpan"
            $('#btnSave').on('click', function() {
                // Ambil nilai input dari form
                var nama = $('#nama').val();
                var nim = $('#nim').val();
                var kelas = $('#kelas').val();

                // Lakukan sesuatu dengan data yang diambil
                console.log("Nama: " + nama);
                console.log("NIM: " + nim);
                console.log("Kelas: " + kelas);


                    // Lakukan permintaan AJAX
                $.ajax({
                    url: 'update_data.php', // Ganti dengan URL ke skrip PHP Anda
                    method: 'POST',
                    data: {
                        nama: nama,
                        nim: nim,
                        kelas: kelas
                    },
                    success: function(response) {
                        // Tangani respons dari skrip PHP
                        console.log(response);
                        // Lakukan sesuatu setelah berhasil mengupdate data, jika diperlukan
                    },
                    error: function(xhr, status, error) {
                        // Tangani kesalahan permintaan AJAX
                        console.error(xhr.responseText);
                    }
                });    



                // Sembunyikan modal
                $('#updateModal').modal('hide');
            });
        });
    </script>
  </body>
</html>
