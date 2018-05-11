const fs = require('fs')

const exportPrefix      = 'm2o_export'
const exportRegionStart = '//@ M2O_EXPORT_REGION_START'
const exportRegionEnd   = '//@ M2O_EXPORT_REGION_END'

const targets = [
    'server/api/general.h',
    'server/api/errors.h',
    'server/api/arguments.h',
    'server/api/plugin.h',
    'server/api/vehicle.h',
    'server/api/pedestrian.h',
];

const getStructs = data => {
    const results = []

    if (data.indexOf(exportRegionStart) === -1) {
        return results
    }

    const start = data.indexOf(exportRegionStart)
    const end   = data.indexOf(exportRegionEnd)

    if (end === -1) {
        throw "couldnt find matching " + exportRegionEnd + " tag"
    }

    results.push(data
        .substring(start, end)
        .replace(exportRegionStart, '')
        .split('\n')
            .map((line, i) => i !== 0 ? ' '.repeat(8) + line : line)
            .join('\n')
    )

    return results.concat(getStructs(data.slice(end + 1)))
}

const replaceTypesInline = line => {
    return line
        .replace('u8',  'unsigned char')
        .replace('u16', 'unsigned short')
        .replace('u32', 'unsigned int')
        .replace('u64', 'unsigned long')
        .replace('i8',  'char')
        .replace('i16', 'short')
        .replace('i32', 'int')
        .replace('i64', 'long')
        .replace('f32', 'float')
        .replace('f64', 'double')
        .replace('b8',  'bool')
        .replace('b64', 'bool')
        .replace('usize', 'size_t')
}

const replaceTypes = data => {
    return data
        .split('\n')
        .map(replaceTypesInline)
        .join('\n')
}

(function main() {
    /* parse the `exportPrefix` file lines */
    const results = targets
        .map(file => fs.readFileSync(file, 'utf8'))
        .map(data => data
            .split('\n')
            .filter(line => line.indexOf(exportPrefix) !== -1)
            .map(line => line
                .replace('{', '')
                .replace(exportPrefix, '')
                .trim()
            )
        )

    const structs = targets
        .map(file => fs.readFileSync(file, 'utf8'))
        .map(replaceTypes)
        .map(data => getStructs(data)
            .reduce((carry, elem) => carry.concat(elem), [])
        )

    const matches = results
        .reduce((carry, elem) => carry.concat(elem), [])
        .map(line => line.match(/m2o_([_a-zA-Z0-9]+)\(/))

    /* generate public API headers */
    const vtableSignatures = [
        matches
            .map(match => match.input.replace(match[0], `(m2o_api_${match[1]})(`))
            .map(method => `typedef ${method};`)
            .join('\n'),
        '',
        'typedef struct m2o_api_vtable {',
        matches
            .map(match => `m2o_api_${match[1]} *${match[1]};`)
            .map(line => ' '.repeat(4) + line)
            .join('\n'),
        '} m2o_api_vtable;',
    ].join('\n');

    const publicHeaderContent = `
        /* Mafia 2 Online Server API header definitions */
        /* Generated on ${(new Date())} */

        #ifndef M2O_API_H
        #define M2O_API_H

        #if defined(__cplusplus)
        #define M2O_EXTERN extern "C"
        #else
        #define M2O_EXTERN extern
        #endif

        #if defined(_WIN32)
        #define M2O_DLL_EXPORT M2O_EXTERN __declspec(dllexport)
        #else
        #define M2O_DLL_EXPORT M2O_EXTERN __attribute__((visibility("default")))
        #endif

        #define M2O_PLUGIN_MAIN(API, PLUGIN) M2O_DLL_EXPORT void m2o_plugin_main(const m2o_api_vtable *API, m2o_plugin *PLUGIN)

        #if defined(__cplusplus)
        extern "C" {
        #endif

        /* definitions */
        ${structs.join('')}

        /* public methods */
        ${replaceTypes(vtableSignatures)}

        #if defined(__cplusplus)
        }
        #endif
        #endif // M2O_API_H`;

    fs.writeFileSync('binary/include/m2o_api.h', publicHeaderContent
        .split('\n')
        .map(s => s.replace(' '.repeat(8), ''))
        .join('\n')
        .trim() + '\n'
    )


    /* generate the internal headers */
    const internalHeader = results
        .map(lines => lines
            .map(line =>  line + ';')
            .join('\n')
        )
        .join('\n\n')


    /* generate api vtable implementation */
    const apiImplementation = [
        internalHeader,
        '',
        vtableSignatures,
        '',
        'void m2o_api_init(m2o_api_vtable *api) {',
        matches
            .map(match => `api->${match[1]} = m2o_${match[1]};`)
            .map(line => ' '.repeat(4) + line)
            .join('\n'),
        '}',
    ];

    fs.writeFileSync('server/api/api.h', apiImplementation.join('\n'))
})()
