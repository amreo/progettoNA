<?php
require('fpdf.php');


$pdf = new FPDF();
$pdf->AddPage();
$pdf->SetFont('Arial','B',60);

$pdf->Text(20,50,"ALIMENTATORE");
$pdf->Text(20,90,"ID PRODOTTO = 3");
$pdf->Text(20,130,"BARCODE:");
$pdf->Image("Alimentatore.jpg",20,160,180);
$pdf->Output();
?>
