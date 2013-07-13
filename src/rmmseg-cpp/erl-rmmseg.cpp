//
// erl-rmmseg.cpp
// Kevin Lynx
// 07.12.2013
//
#include <erl_nif.h>
#include <vector>
#include "dict.h"
#include "algor.h"
#include "token.h"

extern "C"{
static ERL_NIF_TERM load_dicts(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[]) 
{
    char file[512];
    enif_get_string(env, argv[0], file, sizeof(file), ERL_NIF_LATIN1);
    bool ret = rmmseg::dict::load_chars(file);
    if (!ret)
        return enif_make_badarg(env);
    enif_get_string(env, argv[1], file, sizeof(file), ERL_NIF_LATIN1);
    ret = rmmseg::dict::load_words(file);
    if (!ret)
        return enif_make_badarg(env);
    return enif_make_atom(env, "ok");
}

static ERL_NIF_TERM seg(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    ErlNifBinary bin; 
    enif_inspect_binary(env, argv[0], &bin);     
    char *text = (char*) bin.data;
    size_t size = bin.size;
    rmmseg::Algorithm algo(text, size);
    std::vector<ERL_NIF_TERM> terms;
    for (rmmseg::Token tok = algo.next_token(); tok.length != 0; tok = algo.next_token()) {
        ErlNifBinary binText;
        enif_alloc_binary(tok.length, &binText);
        memcpy(binText.data, tok.text, tok.length);
        terms.push_back(enif_make_binary(env, &binText));
    }
    return enif_make_list_from_array(env, &terms[0], terms.size());
}

//
static char *load(int *size) {
    FILE *fp = fopen("input.txt", "r");
    fseek(fp, 0, SEEK_END);
    unsigned pos = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    char *s = (char*) malloc(pos + 1);
    fread(s, pos, 1, fp);
    s[pos] = 0;
    fclose(fp);
    *size = pos;
    return s;
}

static ERL_NIF_TERM sample(ErlNifEnv *env, int argc, const ERL_NIF_TERM argv[])
{
    rmmseg::dict::load_chars("../../data/chars.dic");
    rmmseg::dict::load_words("../../data/words.dic");

    int size;
    char *text = load(&size);
    rmmseg::Algorithm algo(text, size);
    FILE *fp = fopen("output.txt", "a+");
    for (rmmseg::Token tok = algo.next_token(); tok.length != 0; tok = algo.next_token()) {
        char s[128] = {0};
        strncpy(s, tok.text, tok.length);
        fprintf(fp, "%s\n", s);
    }
    free(text);
    fclose(fp);
    return enif_make_atom(env, "ok");
}

//
static ErlNifFunc mynif_funcs[] =
{
    {"load_dicts", 2, load_dicts},
    {"seg", 1, seg},
    {"sample", 0, sample},
};
}
ERL_NIF_INIT(rmmseg, mynif_funcs, NULL, NULL, NULL, NULL);

