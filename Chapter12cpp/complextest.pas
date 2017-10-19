PROGRAM ComplexTest;

VAR
    x, y, z : complex;
    a, b, c : integer;

BEGIN
  x.re := 12;
  x.im := 23;
  y.re := 10;
  y.im := 5;

  z := x + y;

END.
