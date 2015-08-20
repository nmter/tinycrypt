
/*
 *  Copyright (C) 2015 by Intel Corporation, All Rights Reserved.
 * 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are met:
 *
 *    - Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimer.
 *
 *    - Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 *    - Neither the name of Intel Corporation nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 *  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 *  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
 *  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 *  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 *  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 *  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 *  OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 *  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *  test_hmac_prng.c -- Implementation of some HMAC-PRNG tests.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hmac_prng.h"

static void fatal (const char *s) {
  (void) fprintf (stderr, "%s\n", s);
  exit (-1);
}

void show (const char *label, const uint8_t *s, size_t len) {
  uint32_t i;
  printf ("%s = ", label);
  for (i = 0; i < (uint32_t) len; ++i) {
    printf ("%02x", s[i]);
  }
  printf ("\n");
}

void printBinaryFile(const uint8_t *s, uint32_t slen) {
	FILE *write_ptr;
	write_ptr = fopen("pseudo-random-data.bin","wb");
	fwrite(s, slen, 1, write_ptr);
}

int32_t main (void) {

  printf ("Performing HMAC-PRNG tests...\n");
  printf ("\tPerforming HMAC-PRNG test#1 (init, reseed, generate)...");

  uint8_t seed[128];
  struct hmac_prng_struct h;
  uint32_t size = (1 << 19);
  uint8_t random[size];
  uint32_t i;

  // Fake seed (replace by a a truly random seed):
  for (i = 0; i < (uint32_t) sizeof (seed); ++i) {
    seed[i] = i;
  }
  //  Fake personalization and additional_input (replace by appropriate values):
  uint8_t *personalization = (uint8_t *) "HOSTNAME";//e.g.: hostname+timestamp
  uint8_t *additional_input = (uint8_t *) "additional input";

  if (hmac_prng_init (&h, personalization, sizeof (personalization)) == 0) {
    fatal ("HMAC-PRNG initialization failed.");
  }

  if (hmac_prng_reseed (&h, seed, sizeof (seed), additional_input,
      sizeof (additional_input)) == 0) {
    fatal ("HMAC-PRNG reseed failed.");
  }

  if (hmac_prng_generate (random, size, &h) < 1) {
    fatal ("HMAC-PRNG generate failed.");
  }
  //printBinaryFile(random, size);
  //show ("Pseudo-random data", random, size);

  printf("Success!\n");
  printf ("All HMAC tests succeeded!\n\n");

  return 0;
}
