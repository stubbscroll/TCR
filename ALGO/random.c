


/*  initialize an array with a random permutation */
To initialize an array a of n elements to a randomly shuffled copy of source, both 0-based:
  a[0] ? source[0]
  for i from 1 to n - 1 do
      j ? random integer with 0 = j = i
      a[i] ? a[j]'
      a[j] ? source[i]
