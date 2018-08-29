package com.company;

import java.io.*;
import java.util.*;

public class Main {

    private static final char[] letters = {'e', 'h', 'i', 'k', 'l', 'r', 's', 't'};
    private static final HashMap<Character, Integer> code = new HashMap<Character, Integer>() {{
        put('e', 0);
        put('h', 1);
        put('i', 2);
        put('k', 3);
        put('l', 4);
        put('r', 5);
        put('s', 6);
        put('t', 7);
    }};
    private static final String ciphertextA = "khhltk";
    private static final String ciphertextB = "kthlle";

    public static void main(String[] args) {
        List<String> data = buildData();

        try (PrintWriter out = new PrintWriter("out.txt")) {
            for (String candidatePlaintextA : data) {
                String candidateKey = xor(ciphertextA, candidatePlaintextA);
                String candidatePlaintextB = xor(ciphertextB, candidateKey);
                out.println(candidatePlaintextA + "\t" + candidatePlaintextB + "\t" + candidateKey);
            }
        } catch (IOException e) {
            System.out.println(e.getStackTrace());
        }

//        List<String> keys = new ArrayList<String>();
//        for (String word : data) {
//            keys.add(xor(ciphertextA, word));
//        }
//
//        List<String> dict = readDictionary();
//
//        //List possiblePlaintextB = new ArrayList<String>();
//        int maxScore = 0;
//        for (String key : keys) {
//            //possiblePlaintextB.add(xor(ciphertextB, key));
//            String candidate = xor(ciphertextB, key);
//            int score = computeScore(candidate, dict);
//            if ( score > maxScore) {
//                System.out.println(candidate + " " + score);
//                maxScore = score;
//            }
//        }
    }

    private static String xor(String a, String b) {
        assert(a.length() == b.length());
        String result = "";
        for (int i = 0; i < a.length(); i++) {
            char charA = a.charAt(i);
            int codeA = code.get(charA);
            char charB = b.charAt(i);
            int codeB = code.get(charB);
            int xorResult = codeA ^ codeB;
            assert (xorResult > 0 && xorResult < 8);
            result += letters[xorResult];
        }
        return result;
    }

    private static List<String> buildData() {
        List<String> data = new ArrayList<String>();
        foo(letters, "", 6, data);
        return data;
    }

    private static void foo(char[] set, String prefix, int k, List<String> data) {
        if (k == 0) {
            data.add(prefix);
        } else {
            for (int i = 0; i < set.length; ++i) {
                String newPrefix = prefix + set[i];
                foo(set, newPrefix, k - 1, data);
            }
        }
    }

    private static List<String> readDictionary() {
        InputStream is = new Main().getClass().getResourceAsStream("words_alpha.txt");
        List<String> words = new ArrayList<>();
        try {
            BufferedReader reader = new BufferedReader(new InputStreamReader(is));
            String word;
            if (is != null) {
                while ((word = reader.readLine()) != null) {
                    words.add(word);
                }
            }
        } catch (IOException e) {
            System.out.println("IOException");
        } finally {
            try { is.close(); } catch (Throwable ignore) {}
        }
        return words;
    }

    private static boolean contains(List<String> list, String item) {
        int idx = Collections.binarySearch(list, item);
        return (idx > 0);
    }

    private static int computeScore(String text, List<String> words) {
        int score = 0;
        for (String word : words) {
            int lastIndex = 0;
            while(lastIndex != -1){
                lastIndex = text.indexOf(word, lastIndex);
                if(lastIndex != -1){
                    score++;
                    lastIndex += word.length();
                }
            }
        }
        return score;
    }

}
