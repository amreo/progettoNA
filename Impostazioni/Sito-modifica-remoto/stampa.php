<?php
require('fpdf.php');
$prodotto=$_post["prodotto"]

$pdf = new FPDF();
$pdf->AddPage();
$pdf->SetFont('Arial','B',60);

$pdf->Text(20,50,$prodotto);
$pdf->Text(20,100,"BARCODE:");

//INSERIRE IMMAGINE BARCODE

$pdf->Output();
?>
