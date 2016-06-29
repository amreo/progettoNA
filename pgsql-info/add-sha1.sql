CREATE OR REPLACE FUNCTION sha1(text) returns text AS $$
	SELECT encode(digest($1, 'sha1'), 'hex')
$$ LANGUAGE SQL STRICT IMMUTABLE;
