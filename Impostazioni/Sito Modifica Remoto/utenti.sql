CREATE TABLE `login` (
`id` INT( 11 ) NOT NULL AUTO_INCREMENT ,
`username` VARCHAR( 64 ) NOT NULL ,
`password` VARCHAR( 64 ) NOT NULL ,
PRIMARY KEY ( `id` )
) ENGINE = MYISAM
 
INSERT INTO `login` (
`id` ,
`username` ,
`password`
)
VALUES (
NULL , 'tecnicoautorizzato', sha1( 'pwddifficile' )
);